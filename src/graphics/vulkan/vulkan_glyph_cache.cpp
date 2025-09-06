#include "vulkan_glyph_cache.h"

#include <utils/hash.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdexcept>
#include <cmath>

namespace karin
{
VulkanGlyphCache::VulkanGlyphCache(
    VulkanGraphicsDevice* device,
    FontLoader* fontLoader,
    size_t maxFramesInFlight
) : m_device(device), m_fontLoader(fontLoader), m_maxFramesInFlight(maxFramesInFlight)
{
    if (!m_device)
    {
        throw std::runtime_error("VulkanGlyphCache: device is null");
    }
    if (!m_fontLoader)
    {
        throw std::runtime_error("VulkanGlyphCache: fontLoader is null");
    }

    createDescriptorSetLayout();
    createSampler();
    createAtlas();
}

VulkanGlyphCache::~VulkanGlyphCache()
{
    vmaDestroyImage(m_device->allocator(), m_atlasImage, m_atlasImageAllocation);
    vkDestroyImageView(m_device->device(), m_atlasImageView, nullptr);
    vkDestroySampler(m_device->device(), m_atlasSampler, nullptr);
    vkDestroyDescriptorSetLayout(m_device->device(), m_atlasDescriptorSetLayout, nullptr);
}

VulkanGlyphCache::GlyphInfo VulkanGlyphCache::getGlyph(const std::string& character, const Font& font, float size)
{
    if (auto it = m_glyphMap.find(glyphKey(character, font, size)); it != m_glyphMap.end())
    {
        return it->second;
    }

    FT_Face face = m_fontLoader->loadFont(font);
    if (!face)
    {
        throw std::runtime_error("failed to load font face");
    }

    FT_Error error = FT_Set_Char_Size(
        face,
        0,
        static_cast<FT_F26Dot6>(size * 64.0f),
        300,
        300
    );
    if (error)
    {
        throw std::runtime_error("failed to set character size");
    }

    FT_UInt glyphIndex = FT_Get_Char_Index(face, character[0]);
    error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
    if (error)
    {
        throw std::runtime_error("failed to load glyph");
    }

    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    if (error)
    {
        throw std::runtime_error("failed to render glyph");
    }

    FT_GlyphSlot slot = face->glyph;
    Point atlasPos = allocateAtlasSpace(slot->bitmap.width, slot->bitmap.rows);

    GlyphInfo info = {
        .uv = Rectangle(
            atlasPos.x / ATLAS_WIDTH,
            atlasPos.y / ATLAS_HEIGHT,
            static_cast<float>(slot->bitmap.width) / ATLAS_WIDTH,
            static_cast<float>(slot->bitmap.rows) / ATLAS_HEIGHT
        ),
        .width = static_cast<float>(slot->bitmap.width),
        .height = static_cast<float>(slot->bitmap.rows),
        .bearingX = static_cast<float>(slot->bitmap_left),
        .bearingY = static_cast<float>(slot->bitmap_top),
        .advanceX = static_cast<float>(slot->advance.x >> 6),
    };
    m_glyphMap[glyphKey(character, font, size)] = info;

    std::vector<std::byte> bitmapData(slot->bitmap.width * slot->bitmap.rows);
    std::memcpy(bitmapData.data(), slot->bitmap.buffer, bitmapData.size());

    GlyphUploadInfo uploadInfo = {
        .bitmapData = std::move(bitmapData),
        .width = static_cast<int>(slot->bitmap.width),
        .height = static_cast<int>(slot->bitmap.rows),
        .atlasRegion = atlasPos,
    };
    m_uploadQueue.push_back(std::move(uploadInfo));

    return info;
}

void VulkanGlyphCache::flushUploadQueue()
{
    if (m_uploadQueue.empty())
    {
        return;
    }

    size_t totalSize = 0;
    for (const auto& upload : m_uploadQueue)
    {
        totalSize += upload.bitmapData.size();
    }

    VkBuffer stagingBuffer;
    VmaAllocation stagingBufferMemory;
    VmaAllocationCreateInfo stagingBufferAllocationInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };
    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = totalSize,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    if (vmaCreateBuffer(
        m_device->allocator(), &bufferInfo, &stagingBufferAllocationInfo, &stagingBuffer, &stagingBufferMemory, nullptr
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create staging buffer for glyph upload");
    }

    void* mappedData;
    if (vmaMapMemory(m_device->allocator(), stagingBufferMemory, &mappedData) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to map memory for glyph upload staging buffer");
    }

    for (const auto& upload : m_uploadQueue)
    {
        std::memcpy(mappedData, upload.bitmapData.data(), upload.bitmapData.size());
        mappedData = static_cast<std::byte*>(mappedData) + upload.bitmapData.size();
    }
    vmaUnmapMemory(m_device->allocator(), stagingBufferMemory);

    VkCommandBuffer commandBuffer = m_device->beginSingleTimeCommands();
    transitionLayout(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    size_t offset = 0;
    for (const auto& upload : m_uploadQueue)
    {
        VkBufferImageCopy region = {
            .bufferOffset = offset,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = 0,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
            .imageOffset = {
                static_cast<int32_t>(upload.atlasRegion.x),
                static_cast<int32_t>(upload.atlasRegion.y),
                0,
            },
            .imageExtent = {
                static_cast<uint32_t>(upload.width),
                static_cast<uint32_t>(upload.height),
            },
        };
        vkCmdCopyBufferToImage(
            commandBuffer, stagingBuffer, m_atlasImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region
        );
    }

    transitionLayout(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    m_device->endSingleTimeCommands(commandBuffer);

    vmaDestroyBuffer(m_device->allocator(), stagingBuffer, stagingBufferMemory);
}

size_t VulkanGlyphCache::glyphKey(const std::string& character, const Font& font, float size)
{
    auto sizeInt = static_cast<uint32_t>(std::round(size * SIZE_FLOAT_ACCURACY));

    size_t seed = 0;

    hash_combine(seed, character);
    hash_combine(seed, font.hash());
    hash_combine(seed, sizeInt);

    return seed;
}

Point VulkanGlyphCache::allocateAtlasSpace(int width, int height)
{
    if (m_currentX + width > ATLAS_WIDTH)
    {
        m_currentX = 0.0f;
        m_currentY += m_rowHeight;
        m_rowHeight = 0.0f;
    }

    Point position(m_currentX, m_currentY);
    m_currentX += width;
    m_rowHeight = std::max(m_rowHeight, height);
    if (m_currentY + m_rowHeight > ATLAS_HEIGHT)
    {
        throw std::runtime_error("glyph atlas is full");
    }

    return position;
}

void VulkanGlyphCache::createAtlas()
{
    VmaAllocationCreateInfo imageAllocInfo = {
        .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
    };
    VkImageCreateInfo imageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = VK_FORMAT_R8_UNORM,
        .extent = {ATLAS_WIDTH, ATLAS_HEIGHT, 1},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    if (vmaCreateImage(
        m_device->allocator(), &imageCreateInfo, &imageAllocInfo, &m_atlasImage, &m_atlasImageAllocation, nullptr
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create glyph atlas image");
    }

    VkImageViewCreateInfo viewInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = m_atlasImage,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_R8_UNORM,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };
    if (vkCreateImageView(m_device->device(), &viewInfo, nullptr, &m_atlasImageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create glyph atlas image view");
    }

    std::vector layouts(m_maxFramesInFlight, m_atlasDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = m_device->descriptorPool(),
        .descriptorSetCount = static_cast<uint32_t>(m_maxFramesInFlight),
        .pSetLayouts = layouts.data(),
    };
    m_atlasDescriptorSets.resize(m_maxFramesInFlight);
    if (vkAllocateDescriptorSets(m_device->device(), &allocInfo, m_atlasDescriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets for glyph atlas");
    }

    for (size_t i = 0; i < m_maxFramesInFlight; i++)
    {
        VkDescriptorImageInfo imageInfo = {
            .sampler = m_atlasSampler,
            .imageView = m_atlasImageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };
        VkWriteDescriptorSet descriptorWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = m_atlasDescriptorSets[i],
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &imageInfo,
        };
        vkUpdateDescriptorSets(m_device->device(), 1, &descriptorWrite, 0, nullptr);
    }
}

void VulkanGlyphCache::createDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding samplerLayoutBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr,
    };
    VkDescriptorSetLayoutCreateInfo layoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &samplerLayoutBinding,
    };
    if (vkCreateDescriptorSetLayout(
        m_device->device(), &layoutInfo, nullptr, &m_atlasDescriptorSetLayout
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout for glyph atlas");
    }
}

void VulkanGlyphCache::createSampler()
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(m_device->physicalDevice(), &properties);

    VkSamplerCreateInfo samplerInfo = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .flags = 0,
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        .mipLodBias = 0.0f,
        .anisotropyEnable = VK_TRUE,
        .maxAnisotropy = properties.limits.maxSamplerAnisotropy,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .minLod = 0.0f,
        .maxLod = 0.0f,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE,
    };
    if (vkCreateSampler(m_device->device(), &samplerInfo, nullptr, &m_atlasSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create glyph atlas sampler");
    }
}

void VulkanGlyphCache::transitionLayout(VkCommandBuffer commandBuffer, VkImageLayout newLayout)
{
    if (m_atlasImageLayout == newLayout)
    {
        return;
    }

    VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = 0,
        .dstAccessMask = 0,
        .oldLayout = m_atlasImageLayout,
        .newLayout = newLayout,
        .image = m_atlasImage,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
    if (m_atlasImageLayout == VK_IMAGE_LAYOUT_UNDEFINED
        && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (m_atlasImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (m_atlasImageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else
    {
        throw std::invalid_argument("unsupported layout transition");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    m_atlasImageLayout = newLayout;
}
} // karin
