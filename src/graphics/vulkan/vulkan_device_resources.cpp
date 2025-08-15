#include "vulkan_device_resources.h"

#include "vma.h"

#include <karin/common/color/color.h>
#include <ranges>
#include <cstring>

namespace karin
{
void VulkanDeviceResources::cleanup()
{
    for (auto& val : m_gradientPointLutMap | std::views::values)
    {
        vmaDestroyImage(m_device->allocator(), val.image, val.allocation);
        vkDestroyImageView(m_device->device(), val.imageView, nullptr);
    }

    m_gradientPointLutMap.clear();

    vkDestroyDescriptorSetLayout(m_device->device(), m_gradientPointLutDescriptorSetLayout, nullptr);

    vkDestroySampler(m_device->device(), m_clampSampler, nullptr);
    vkDestroySampler(m_device->device(), m_repeatSampler, nullptr);
    vkDestroySampler(m_device->device(), m_mirrorSampler, nullptr);
}

std::vector<VkDescriptorSet> VulkanDeviceResources::gradientPointLutDescriptorSet(
    const LinearGradientPattern& pattern
)
{
    if (auto it = m_gradientPointLutMap.find(pattern.pointsHash()); it != m_gradientPointLutMap.end())
    {
        return it->second.descriptorSets;
    }

    auto data = generateGradientPointLut(pattern.gradientPoints);

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
        .imageType = VK_IMAGE_TYPE_1D,
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
        .viewType = VK_IMAGE_VIEW_TYPE_1D,
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

    std::vector layouts(m_maxFramesInFlight, m_gradientPointLutDescriptorSetLayout);
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
    switch (pattern.extendMode)
    {
    case LinearGradientPattern::ExtendMode::CLAMP:
        gradientPointLutSampler = m_clampSampler;
        break;
    case LinearGradientPattern::ExtendMode::REPEAT:
        gradientPointLutSampler = m_repeatSampler;
        break;
    case LinearGradientPattern::ExtendMode::MIRROR:
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

    LutTexture lutTexture = {
        .image = gradientPointLutImage,
        .allocation = gradientPointLutImageAllocation,
        .imageView = gradientPointLutImageView,
        .descriptorSets = std::move(descriptorSets),
    };
    m_gradientPointLutMap[pattern.pointsHash()] = lutTexture;

    return lutTexture.descriptorSets;
}

std::array<uint8_t, VulkanDeviceResources::LUT_WIDTH * 4> VulkanDeviceResources::generateGradientPointLut(
    const std::vector<LinearGradientPattern::GradientPoint>& gradientPoints
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

VkDescriptorSetLayout VulkanDeviceResources::gradientPointLutDescriptorSetLayout() const
{
    return m_gradientPointLutDescriptorSetLayout;
}

void VulkanDeviceResources::createGradientPointLutDescriptorSetLayout()
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
        m_device->device(), &layoutInfo, nullptr, &m_gradientPointLutDescriptorSetLayout
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout for linear gradient pipeline");
    }
}
} // karin
