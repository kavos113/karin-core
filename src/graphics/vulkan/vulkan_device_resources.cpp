#include "vulkan_device_resources.h"

#include <karin/common/color/color.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

#include <ranges>
#include <cstring>
#include <stdexcept>
#include <functional>
#include <string_view>

namespace karin
{
void VulkanDeviceResources::cleanup()
{
    for (auto& val : m_gradientPointLutMap | std::views::values)
    {
        vmaDestroyImage(m_device->allocator(), val.image, val.allocation);
        vkDestroyImageView(m_device->device(), val.imageView, nullptr);
    }

    for (auto& val : m_textureMap | std::views::values)
    {
        vmaDestroyImage(m_device->allocator(), val.image, val.allocation);
        vkDestroyImageView(m_device->device(), val.imageView, nullptr);
    }

    vmaDestroyImage(m_device->allocator(), m_dummyTexture.image, m_dummyTexture.allocation);
    vkDestroyImageView(m_device->device(), m_dummyTexture.imageView, nullptr);

    m_gradientPointLutMap.clear();

    vkDestroyDescriptorSetLayout(m_device->device(), m_geometryDescriptorSetLayout, nullptr);

    vkDestroySampler(m_device->device(), m_clampSampler, nullptr);
    vkDestroySampler(m_device->device(), m_repeatSampler, nullptr);
    vkDestroySampler(m_device->device(), m_mirrorSampler, nullptr);
}

std::vector<VkDescriptorSet> VulkanDeviceResources::gradientPointLutDescriptorSet(
    const GradientPoints& points
)
{
    if (auto it = m_gradientPointLutMap.find(points.hash()); it != m_gradientPointLutMap.end())
    {
        return it->second.descriptorSets;
    }

    auto data = generateGradientPointLut(points.points);

    VkBuffer stagingBuffer;
    VmaAllocation stagingBufferMemory;
    VmaAllocationCreateInfo stagingBufferAllocationInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };
    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = data.size(),
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    if (vmaCreateBuffer(
        m_device->allocator(), &bufferInfo, &stagingBufferAllocationInfo, &stagingBuffer, &stagingBufferMemory, nullptr
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create staging buffer for gradient point LUT");
    }

    void* mappedData;
    if (vmaMapMemory(m_device->allocator(), stagingBufferMemory, &mappedData) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to map patternory for gradient point LUT");
    }
    memcpy(mappedData, data.data(), data.size());
    vmaUnmapMemory(m_device->allocator(), stagingBufferMemory);


    VmaAllocationCreateInfo gradientPointLutAllocationInfo = {
        .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };
    VkImageCreateInfo imageInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_UNORM,
        .extent = {LUT_WIDTH, 1, 1},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    VkImage gradientPointLutImage;
    VmaAllocation gradientPointLutImageAllocation;
    if (vmaCreateImage(
        m_device->allocator(), &imageInfo, &gradientPointLutAllocationInfo, &gradientPointLutImage,
        &gradientPointLutImageAllocation, nullptr
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create gradient point LUT image");
    }

    VkCommandBuffer commandBuffer = m_device->beginSingleTimeCommands();

    VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .image = gradientPointLutImage,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };
    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
        0, 0, nullptr,
        0, nullptr,
        1, &barrier
    );

    VkBufferImageCopy region = {
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
        .imageOffset = {0, 0, 0},
        .imageExtent = {LUT_WIDTH, 1, 1},
    };
    vkCmdCopyBufferToImage(
        commandBuffer, stagingBuffer, gradientPointLutImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region
    );

    barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .image = gradientPointLutImage,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };
    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0, 0, nullptr,
        0, nullptr,
        1, &barrier
    );

    m_device->endSingleTimeCommands(commandBuffer);

    vmaDestroyBuffer(m_device->allocator(), stagingBuffer, stagingBufferMemory);

    VkImageViewCreateInfo viewInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = gradientPointLutImage,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_UNORM,
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
    VkImageView gradientPointLutImageView;
    if (vkCreateImageView(m_device->device(), &viewInfo, nullptr, &gradientPointLutImageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create gradient point LUT image view");
    }

    std::vector layouts(m_maxFramesInFlight, m_geometryDescriptorSetLayout);
    std::vector<VkDescriptorSet> descriptorSets(m_maxFramesInFlight);

    VkDescriptorSetAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = m_device->descriptorPool(),
        .descriptorSetCount = m_maxFramesInFlight,
        .pSetLayouts = layouts.data(),
    };
    if (vkAllocateDescriptorSets(m_device->device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets for gradient point LUT");
    }

    VkSampler gradientPointLutSampler;
    switch (points.extendMode)
    {
    case ExtendMode::CLAMP:
        gradientPointLutSampler = m_clampSampler;
        break;
    case ExtendMode::REPEAT:
        gradientPointLutSampler = m_repeatSampler;
        break;
    case ExtendMode::MIRROR:
        gradientPointLutSampler = m_mirrorSampler;
        break;
    default:
        throw std::runtime_error("unsupported extend mode for gradient point LUT");
    }

    for (size_t i = 0; i < m_maxFramesInFlight; ++i)
    {
        VkDescriptorImageInfo descriptorImageInfo = {
            .sampler = gradientPointLutSampler,
            .imageView = gradientPointLutImageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };

        VkWriteDescriptorSet descriptorWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptorSets[i],
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &descriptorImageInfo,
        };
        vkUpdateDescriptorSets(m_device->device(), 1, &descriptorWrite, 0, nullptr);
    }

    Texture lutTexture = {
        .image = gradientPointLutImage,
        .allocation = gradientPointLutImageAllocation,
        .imageView = gradientPointLutImageView,
        .descriptorSets = std::move(descriptorSets),
    };
    m_gradientPointLutMap[points.hash()] = lutTexture;

    return lutTexture.descriptorSets;
}

std::array<uint8_t, VulkanDeviceResources::LUT_WIDTH * 4> VulkanDeviceResources::generateGradientPointLut(
    const std::vector<GradientPoints::GradientPoint>& gradientPoints
) const
{
    std::array<uint8_t, LUT_WIDTH * 4> lut = {};

    size_t pointIndex = 0;
    for (int i = 0; i < LUT_WIDTH; ++i)
    {
        float t = static_cast<float>(i) / (LUT_WIDTH - 1);
        while (pointIndex < gradientPoints.size() - 2 && t > gradientPoints[pointIndex + 1].offset)
        {
            ++pointIndex;
        }

        if (pointIndex == gradientPoints.size() - 1)
        {
            lut[i * 4 + 0] = static_cast<uint8_t>(gradientPoints.back().color.r);
            lut[i * 4 + 1] = static_cast<uint8_t>(gradientPoints.back().color.g);
            lut[i * 4 + 2] = static_cast<uint8_t>(gradientPoints.back().color.b);
            lut[i * 4 + 3] = static_cast<uint8_t>(gradientPoints.back().color.a);
        }
        else
        {
            const auto& p0 = gradientPoints[pointIndex];
            const auto& p1 = gradientPoints[pointIndex + 1];

            float localT = (t - p0.offset) / (p1.offset - p0.offset);
            Color color = lerp(p0.color, p1.color, localT);

            lut[i * 4 + 0] = static_cast<uint8_t>(color.r * 255);
            lut[i * 4 + 1] = static_cast<uint8_t>(color.g * 255);
            lut[i * 4 + 2] = static_cast<uint8_t>(color.b * 255);
            lut[i * 4 + 3] = static_cast<uint8_t>(color.a * 255);
        }
    }

    return lut;
}

void VulkanDeviceResources::createSamplers()
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
    if (vkCreateSampler(m_device->device(), &samplerInfo, nullptr, &m_clampSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create sampler");
    }

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    if (vkCreateSampler(m_device->device(), &samplerInfo, nullptr, &m_repeatSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create sampler");
    }

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    if (vkCreateSampler(m_device->device(), &samplerInfo, nullptr, &m_mirrorSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create sampler");
    }
}

std::vector<VkDescriptorSet> VulkanDeviceResources::textureDescriptorSet(Image image)
{
    if (auto it = m_textureMap.find(image.hash()); it != m_textureMap.end())
    {
        return it->second.descriptorSets;
    }

    throw std::runtime_error("texture not found in VulkanDeviceResources");
}

VkDescriptorSetLayout VulkanDeviceResources::geometryDescriptorSetLayout() const
{
    return m_geometryDescriptorSetLayout;
}

VkDescriptorSetLayout VulkanDeviceResources::textDescriptorSetLayout() const
{
    return m_textDescriptorSetLayout;
}

Image VulkanDeviceResources::createImage(const std::vector<std::byte>& data, uint32_t width, uint32_t height)
{
    VkBuffer stagingBuffer;
    VmaAllocation stagingBufferMemory;
    VmaAllocationCreateInfo stagingBufferAllocationInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };
    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = data.size(),
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    if (vmaCreateBuffer(
        m_device->allocator(), &bufferInfo, &stagingBufferAllocationInfo, &stagingBuffer, &stagingBufferMemory, nullptr
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create staging buffer for image");
    }

    void* mappedData;
    if (vmaMapMemory(m_device->allocator(), stagingBufferMemory, &mappedData) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to map memory for image staging buffer");
    }
    memcpy(mappedData, data.data(), data.size());
    vmaUnmapMemory(m_device->allocator(), stagingBufferMemory);

    VmaAllocationCreateInfo imageAllocationInfo = {
        .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };
    VkImageCreateInfo imageInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_UNORM,
        .extent = {width, height, 1},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    VkImage image;
    VmaAllocation imageAllocation;
    if (vmaCreateImage(
        m_device->allocator(), &imageInfo, &imageAllocationInfo, &image, &imageAllocation, nullptr
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image");
    }

    VkCommandBuffer commandBuffer = m_device->beginSingleTimeCommands();

    VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .image = image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };
    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
        0, 0, nullptr,
        0, nullptr,
        1, &barrier
    );

    VkBufferImageCopy region = {
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
        .imageOffset = {0, 0, 0},
        .imageExtent = {width, height, 1},
    };
    vkCmdCopyBufferToImage(
        commandBuffer, stagingBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region
    );

    barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .image = image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };
    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0, 0, nullptr,
        0, nullptr,
        1, &barrier
    );
    m_device->endSingleTimeCommands(commandBuffer);

    vmaDestroyBuffer(m_device->allocator(), stagingBuffer, stagingBufferMemory);

    VkImageViewCreateInfo viewInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_UNORM,
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
    VkImageView imageView;
    if (vkCreateImageView(m_device->device(), &viewInfo, nullptr, &imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image view");
    }

    std::vector layouts(m_maxFramesInFlight, m_geometryDescriptorSetLayout);
    std::vector<VkDescriptorSet> descriptorSets(m_maxFramesInFlight);
    VkDescriptorSetAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = m_device->descriptorPool(),
        .descriptorSetCount = m_maxFramesInFlight,
        .pSetLayouts = layouts.data(),
    };
    if (vkAllocateDescriptorSets(m_device->device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets for image");
    }

    for (size_t i = 0; i < m_maxFramesInFlight; ++i)
    {
        VkDescriptorImageInfo descriptorImageInfo = {
            .sampler = m_clampSampler,
            .imageView = imageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };

        VkWriteDescriptorSet descriptorWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptorSets[i],
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &descriptorImageInfo,
        };
        vkUpdateDescriptorSets(m_device->device(), 1, &descriptorWrite, 0, nullptr);
    }


    Texture texture = {
        .image = image,
        .allocation = imageAllocation,
        .imageView = imageView,
        .descriptorSets = std::move(descriptorSets),
    };

    std::string_view dataView(reinterpret_cast<const char*>(data.data()), data.size());
    size_t hash = std::hash<std::string_view>{}(dataView);

    m_textureMap[hash] = texture;
    return Image(hash, width, height);
}

void VulkanDeviceResources::createDescriptorSetLayouts()
{
    VkDescriptorSetLayoutBinding binding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        .pImmutableSamplers = nullptr
    };
    VkDescriptorSetLayoutCreateInfo layoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &binding
    };
    if (vkCreateDescriptorSetLayout(
        m_device->device(), &layoutInfo, nullptr, &m_geometryDescriptorSetLayout
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout for linear gradient pipeline");
    }

    std::array bindings = {
        binding,
        VkDescriptorSetLayoutBinding{
            .binding = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pImmutableSamplers = nullptr
        }
    };
    layoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(bindings.size()),
        .pBindings = bindings.data()
    };
    if (vkCreateDescriptorSetLayout(
        m_device->device(), &layoutInfo, nullptr, &m_textDescriptorSetLayout
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout for text pipeline");
    }
}

std::vector<VulkanDeviceResources::GlyphPosition> VulkanDeviceResources::textLayout(const TextLayout& layout)
{
    if (auto it = m_textLayoutCache.find(layout.hash()); it != m_textLayoutCache.end())
    {
        return it->second;
    }

    FT_Face face = m_fontLoader->loadFont(layout.format.font);
    FT_Error error = FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(layout.format.size));
    if (error)
    {
        throw std::runtime_error("failed to set pixel sizes for font");
    }

    hb_font_t* hbFont = hb_ft_font_create(face, nullptr);
    hb_buffer_t* hbBuffer = hb_buffer_create();

    hb_buffer_add_utf8(hbBuffer, layout.text.c_str(), -1, 0, -1);

    hb_direction_t direction = HB_DIRECTION_LTR;
    switch (layout.format.readingDirection)
    {
    case TextFormat::Direction::LEFT_TO_RIGHT:
        direction = HB_DIRECTION_LTR;
        break;
    case TextFormat::Direction::RIGHT_TO_LEFT:
        direction = HB_DIRECTION_RTL;
        break;
    case TextFormat::Direction::TOP_TO_BOTTOM:
        direction = HB_DIRECTION_TTB;
        break;
    case TextFormat::Direction::BOTTOM_TO_TOP:
        direction = HB_DIRECTION_BTT;
        break;
    }

    hb_buffer_set_direction(hbBuffer, direction);
    hb_buffer_set_language(hbBuffer, hb_language_from_string(layout.format.locale.c_str(), -1));

    hb_shape(hbFont, hbBuffer, nullptr, 0);

    unsigned int glyphCount;
    hb_glyph_info_t* glyphInfo = hb_buffer_get_glyph_infos(hbBuffer, &glyphCount);
    hb_glyph_position_t* glyphPos = hb_buffer_get_glyph_positions(hbBuffer, &glyphCount);

    float penX = 0;
    float penY = 0;

    std::vector<GlyphPosition> glyphs;
    glyphs.reserve(glyphCount);

    for (unsigned int i = 0; i < glyphCount; i++)
    {
        FT_UInt glyphIndex = glyphInfo[i].codepoint;
        VulkanGlyphCache::GlyphInfo gInfo = m_glyphCache->getGlyph(glyphIndex, layout.format.font, layout.format.size);
        // test: dont use hb yet
        GlyphPosition pos;
        pos.uv = gInfo.uv;
        pos.position.pos = Point(penX + gInfo.bearingX, penY - gInfo.bearingY);
        pos.position.size = Size(gInfo.width, gInfo.height);
        glyphs.push_back(pos);
    }

    hb_buffer_destroy(hbBuffer);
    hb_font_destroy(hbFont);

    m_textLayoutCache[layout.hash()] = glyphs;
    return glyphs;
}

std::vector<VkDescriptorSet> VulkanDeviceResources::dummyTextureDescriptorSet() const
{
    return m_dummyTexture.descriptorSets;
}

void VulkanDeviceResources::createDummyTexture()
{
    VkBuffer stagingBuffer;
    VmaAllocation stagingBufferMemory;
    VmaAllocationCreateInfo stagingBufferAllocationInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };
    std::array whitePixel = {std::byte{255}, std::byte{255}, std::byte{255}, std::byte{255}};
    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = whitePixel.size(),
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };
    if (vmaCreateBuffer(
        m_device->allocator(), &bufferInfo, &stagingBufferAllocationInfo, &stagingBuffer, &stagingBufferMemory, nullptr
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create staging buffer for dummy texture");
    }

    void* mappedData;
    if (vmaMapMemory(m_device->allocator(), stagingBufferMemory, &mappedData) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to map memory for dummy texture staging buffer");
    }
    memcpy(mappedData, whitePixel.data(), whitePixel.size());
    vmaUnmapMemory(m_device->allocator(), stagingBufferMemory);

    VmaAllocationCreateInfo imageAllocationInfo = {
        .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };
    VkImageCreateInfo imageInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_UNORM,
        .extent = {1, 1, 1},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    VkImage image;
    VmaAllocation imageAllocation;
    if (vmaCreateImage(
        m_device->allocator(), &imageInfo, &imageAllocationInfo, &image, &imageAllocation, nullptr
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create dummy texture image");
    }

    VkCommandBuffer commandBuffer = m_device->beginSingleTimeCommands();

    VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .image = image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };
    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
        0, 0, nullptr,
        0, nullptr,
        1, &barrier
    );

    VkBufferImageCopy region = {
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
        .imageOffset = {0, 0, 0},
        .imageExtent = {1, 1, 1},
    };
    vkCmdCopyBufferToImage(
        commandBuffer, stagingBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region
    );

    barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .image = image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };
    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0, 0, nullptr,
        0, nullptr,
        1, &barrier
    );
    m_device->endSingleTimeCommands(commandBuffer);

    vmaDestroyBuffer(m_device->allocator(), stagingBuffer, stagingBufferMemory);

    VkImageViewCreateInfo viewInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_UNORM,
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
    VkImageView imageView;
    if (vkCreateImageView(m_device->device(), &viewInfo, nullptr, &imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image view for dummy texture");
    }

    std::vector layouts(m_maxFramesInFlight, m_geometryDescriptorSetLayout);
    std::vector<VkDescriptorSet> descriptorSets(m_maxFramesInFlight);
    VkDescriptorSetAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = m_device->descriptorPool(),
        .descriptorSetCount = m_maxFramesInFlight,
        .pSetLayouts = layouts.data(),
    };
    if (vkAllocateDescriptorSets(m_device->device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets for dummy texture");
    }

    for (size_t i = 0; i < m_maxFramesInFlight; ++i)
    {
        VkDescriptorImageInfo descriptorImageInfo = {
            .sampler = m_clampSampler,
            .imageView = imageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };

        VkWriteDescriptorSet descriptorWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = descriptorSets[i],
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &descriptorImageInfo,
        };
        vkUpdateDescriptorSets(m_device->device(), 1, &descriptorWrite, 0, nullptr);
    }

    m_dummyTexture = {
        .image = image,
        .allocation = imageAllocation,
        .imageView = imageView,
        .descriptorSets = std::move(descriptorSets),
    };
}
} // karin
