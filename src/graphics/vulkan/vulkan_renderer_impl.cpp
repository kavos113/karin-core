#include "vulkan_renderer_impl.h"

#include "shaders/push_constants.h"
#include "shaders/shaders.h"

#include <algorithm>
#include <iostream>
#include <cstring>
#include <glm/ext/matrix_clip_space.hpp>

namespace karin
{
VulkanRendererImpl::VulkanRendererImpl(
    VulkanGraphicsDevice* device,
    Window::NativeHandle nativeHandle,
    std::unique_ptr<FontLoader> fontLoader
)
    : m_device(device)
{
    m_surface = std::make_unique<VulkanSurface>(m_device, nativeHandle);
    m_extent = m_surface->extent();
    m_deviceResources = std::make_unique<VulkanDeviceResources>(m_device, std::move(fontLoader), MAX_FRAMES_IN_FLIGHT);

    createCommandBuffers();
    createSyncObjects();
    createRenderPass();
    createFrameBuffers();

    createVertexBuffer();
    createIndexBuffer();
    createMatrixBuffer();

    createPipeline();
}

void VulkanRendererImpl::cleanUp()
{
    vkDeviceWaitIdle(m_device->device());

    for (const auto& framebuffer : m_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(m_device->device(), framebuffer, nullptr);
    }
    m_swapChainFramebuffers.clear();

    for (const auto& semaphore : m_swapChainSemaphores)
    {
        vkDestroySemaphore(m_device->device(), semaphore, nullptr);
    }
    m_swapChainSemaphores.clear();

    for (const auto& semaphore : m_finishQueueSemaphores)
    {
        vkDestroySemaphore(m_device->device(), semaphore, nullptr);
    }
    m_finishQueueSemaphores.clear();

    for (const auto& fence : m_swapChainFences)
    {
        vkDestroyFence(m_device->device(), fence, nullptr);
    }
    m_swapChainFences.clear();

    for (auto& commandBuffer : m_commandBuffers)
    {
        vkFreeCommandBuffers(m_device->device(), m_device->commandPool(), 1, &commandBuffer);
    }
    m_commandBuffers.clear();

    vmaDestroyBuffer(m_device->allocator(), m_vertexBuffer, m_vertexAllocation);
    vmaDestroyBuffer(m_device->allocator(), m_indexBuffer, m_indexAllocation);
    for (size_t i = 0; i < m_projMatrixBuffers.size(); ++i)
    {
        vmaDestroyBuffer(m_device->allocator(), m_projMatrixBuffers[i], m_projMatrixBufferAllocations[i]);
    }

    vkDestroyDescriptorSetLayout(m_device->device(), m_projMatrixDescriptorSetLayout, nullptr);

    m_deviceResources->cleanup();

    m_geometryPipeline->cleanUp(m_device->device());
    m_textPipeline->cleanUp(m_device->device());
    vkDestroyRenderPass(m_device->device(), m_renderPass, nullptr);

    m_surface->cleanUp();
}

bool VulkanRendererImpl::beginDraw()
{
    m_vertexMapPoint = m_vertexStartPoint;
    m_indexMapPoint = m_indexStartPoint;
    m_vertexOffset = 0;
    m_indexCount = 0;

    vkWaitForFences(m_device->device(), 1, &m_swapChainFences[m_currentFrame], VK_TRUE, UINT64_MAX);
    m_imageIndex = m_surface->acquireNextImage(m_swapChainSemaphores[m_currentFrame]);
    if (m_imageIndex == -1)
    {
        doResize();
        return false;
    }
    vkResetFences(m_device->device(), 1, &m_swapChainFences[m_currentFrame]);

    vkResetCommandBuffer(m_commandBuffers[m_currentFrame], 0);

    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = nullptr,
    };
    if (vkBeginCommandBuffer(m_commandBuffers[m_currentFrame], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = m_renderPass,
        .framebuffer = m_swapChainFramebuffers[m_imageIndex],
        .renderArea = {
            .offset = {0, 0},
            .extent = m_surface->extent(),
        },
        .clearValueCount = 1,
        .pClearValues = &m_clearColor
    };
    vkCmdBeginRenderPass(m_commandBuffers[m_currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    m_surface->setViewPorts(m_commandBuffers[m_currentFrame]);

    return true;
}

void VulkanRendererImpl::endDraw()
{
    m_deviceResources->flushGlyphUploads();

    std::array vertexBuffers = {m_vertexBuffer};
    std::array<VkDeviceSize, 1> offsets = {};
    vkCmdBindVertexBuffers(
        m_commandBuffers[m_currentFrame], 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data()
    );
    vkCmdBindIndexBuffer(m_commandBuffers[m_currentFrame], m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    std::ranges::sort(
        m_drawCommands,
        [](const DrawCommand& a, const DrawCommand& b)
        {
            return a.pipeline < b.pipeline;
        }
    );

    vkCmdBindPipeline(
        m_commandBuffers[m_currentFrame],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_drawCommands.empty() ? m_geometryPipeline->pipeline() : m_drawCommands.front().pipeline->pipeline()
    );
    VulkanPipeline* lastPipeline = m_drawCommands.empty() ? m_geometryPipeline.get() : m_drawCommands.front().pipeline;

    for (const auto& command : m_drawCommands)
    {
        if (command.pipeline != lastPipeline)
        {
            vkCmdBindPipeline(
                m_commandBuffers[m_currentFrame],
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                command.pipeline->pipeline()
            );
            lastPipeline = command.pipeline;
        }

        vkCmdBindDescriptorSets(
            m_commandBuffers[m_currentFrame],
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            command.pipeline->pipelineLayout(),
            0, command.descriptorSets.size(), command.descriptorSets.data(),
            0, nullptr
        );

        vkCmdPushConstants(
            m_commandBuffers[m_currentFrame],
            command.pipeline->pipelineLayout(),
            VK_SHADER_STAGE_FRAGMENT_BIT,
            0, sizeof(PushConstants), &command.fragData
        );

        vkCmdDrawIndexed(m_commandBuffers[m_currentFrame], command.indexCount, 1, command.indexOffset, 0, 0);
    }

    vkCmdEndRenderPass(m_commandBuffers[m_currentFrame]);

    if (vkEndCommandBuffer(m_commandBuffers[m_currentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer");
    }

    std::array semaphores = {m_swapChainSemaphores[m_currentFrame]};
    std::array signalSemaphores = {m_finishQueueSemaphores[m_imageIndex]};
    std::array<VkPipelineStageFlags, 1> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = static_cast<uint32_t>(semaphores.size()),
        .pWaitSemaphores = semaphores.data(),
        .pWaitDstStageMask = waitStages.data(),
        .commandBufferCount = 1,
        .pCommandBuffers = &m_commandBuffers[m_currentFrame],
        .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()),
        .pSignalSemaphores = signalSemaphores.data(),
    };
    if (vkQueueSubmit(m_device->graphicsQueue(), 1, &submitInfo, m_swapChainFences[m_currentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer");
    }

    bool ret = m_surface->present(m_finishQueueSemaphores[m_imageIndex], m_imageIndex);
    if (!ret)
    {
        doResize();
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanRendererImpl::resize(Size size)
{
}

void VulkanRendererImpl::addCommand(
    const std::vector<VulkanPipeline::Vertex>& vertices,
    std::vector<uint16_t>& indices,
    const PushConstants& fragData,
    const Pattern& pattern,
    bool isGeometry
)
{
    memcpy(m_vertexMapPoint, vertices.data(), vertices.size() * sizeof(VulkanPipeline::Vertex));
    m_vertexMapPoint += vertices.size();

    for (uint16_t& index : indices)
    {
        index += m_vertexOffset;
    }

    memcpy(m_indexMapPoint, indices.data(), indices.size() * sizeof(uint16_t));
    m_indexMapPoint += indices.size();
    m_indexCount += indices.size();

    m_vertexOffset += static_cast<uint16_t>(vertices.size());

    DrawCommand drawCommand = {
        .indexCount = static_cast<uint32_t>(indices.size()),
        .indexOffset = static_cast<uint32_t>(m_indexCount - indices.size()),
        .fragData = fragData,
        .pipeline = isGeometry ? m_geometryPipeline.get() : m_textPipeline.get(),
    };

    std::visit(
        [this, &drawCommand]<typename T0>(const T0& p)
        {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, LinearGradientPattern>)
            {
                auto descriptorSets = m_deviceResources->gradientPointLutDescriptorSet(p.gradientPoints);
                drawCommand.descriptorSets.push_back(descriptorSets[m_currentFrame]);

                VkExtent2D extent = m_surface->extent();
                drawCommand.fragData.global.x = extent.width / static_cast<float>(extent.height);
            }
            else if constexpr (std::is_same_v<T, RadialGradientPattern>)
            {
                auto descriptorSets = m_deviceResources->gradientPointLutDescriptorSet(p.gradientPoints);
                drawCommand.descriptorSets.push_back(descriptorSets[m_currentFrame]);
            }
            else if constexpr (std::is_same_v<T, ImagePattern>)
            {
                auto descriptorSets = m_deviceResources->textureDescriptorSet(p.image);
                drawCommand.descriptorSets.push_back(descriptorSets[m_currentFrame]);
            }
            else if constexpr (std::is_same_v<T, SolidColorPattern>)
            {
                auto descriptorSets = m_deviceResources->dummyTextureDescriptorSet();
                drawCommand.descriptorSets.push_back(descriptorSets[m_currentFrame]);
            }
            else
            {
                throw std::runtime_error("Unsupported pattern type");
            }
        }, pattern
    );

    if (!isGeometry)
    {
        auto glyphAtlasSets = m_deviceResources->glyphAtlasDescriptorSets();
        drawCommand.descriptorSets.push_back(glyphAtlasSets[m_currentFrame]);
    }

    m_drawCommands.push_back(drawCommand);
}

void VulkanRendererImpl::createCommandBuffers()
{
    m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = m_device->commandPool(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size()),
    };

    if (vkAllocateCommandBuffers(m_device->device(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers");
    }
}

void VulkanRendererImpl::createSyncObjects()
{
    m_swapChainSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_finishQueueSemaphores.resize(m_surface->imageCount());
    m_swapChainFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    VkFenceCreateInfo fenceInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(m_device->device(), &semaphoreInfo, nullptr, &m_swapChainSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(m_device->device(), &fenceInfo, nullptr, &m_swapChainFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain sync objects");
        }
    }

    for (size_t i = 0; i < m_surface->imageCount(); i++)
    {
        if (vkCreateSemaphore(m_device->device(), &semaphoreInfo, nullptr, &m_finishQueueSemaphores[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create finish queue semaphore");
        }
    }
}

void VulkanRendererImpl::createVertexBuffer()
{
    VmaAllocationCreateInfo allocInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };

    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = vertexBufferSize,
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VmaAllocationInfo memoryInfo;
    if (vmaCreateBuffer(
        m_device->allocator(), &bufferInfo, &allocInfo, &m_vertexBuffer, &m_vertexAllocation, &memoryInfo
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer");
    }

    m_vertexMapPoint = static_cast<VulkanPipeline::Vertex*>(memoryInfo.pMappedData);
    m_vertexStartPoint = m_vertexMapPoint;
}

void VulkanRendererImpl::createIndexBuffer()
{
    VmaAllocationCreateInfo allocInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };

    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = indexBufferSize,
        .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VmaAllocationInfo memoryInfo;
    if (vmaCreateBuffer(
        m_device->allocator(), &bufferInfo, &allocInfo, &m_indexBuffer, &m_indexAllocation, &memoryInfo
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create index buffer");
    }

    m_indexMapPoint = static_cast<uint16_t*>(memoryInfo.pMappedData);
    m_indexStartPoint = m_indexMapPoint;
}

void VulkanRendererImpl::createMatrixBuffer()
{
    VkDescriptorSetLayoutBinding projMatrixLayoutBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr,
    };
    VkDescriptorSetLayoutCreateInfo layoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &projMatrixLayoutBinding,
    };
    if (vkCreateDescriptorSetLayout(
        m_device->device(), &layoutInfo, nullptr, &m_projMatrixDescriptorSetLayout
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create projection matrix descriptor set layout");
    }

    m_projMatrixData.proj = glm::orthoLH_ZO(
        0.0f,
        static_cast<float>(m_extent.width),
        static_cast<float>(m_extent.height),
        0.0f,
        0.0f,
        1.0f
    );

    m_projMatrixDescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    m_projMatrixBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    m_projMatrixBufferAllocations.resize(MAX_FRAMES_IN_FLIGHT);
    m_projMatrixBufferMemoryInfos.resize(MAX_FRAMES_IN_FLIGHT);

    std::vector layouts(MAX_FRAMES_IN_FLIGHT, m_projMatrixDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfoDesc = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = m_device->descriptorPool(),
        .descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
        .pSetLayouts = layouts.data(),
    };
    if (vkAllocateDescriptorSets(
        m_device->device(), &allocInfoDesc, m_projMatrixDescriptorSets.data()
    ) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate projection matrix descriptor sets");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDeviceSize bufferSize = sizeof(MatrixBufferObject);
        VmaAllocationCreateInfo allocInfo = {
            .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_AUTO,
        };

        VkBufferCreateInfo bufferInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = bufferSize,
            .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };

        if (vmaCreateBuffer(
            m_device->allocator(), &bufferInfo, &allocInfo,
            &m_projMatrixBuffers[i], &m_projMatrixBufferAllocations[i], &m_projMatrixBufferMemoryInfos[i]
        ) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create projection matrix buffer");
        }

        memcpy(
            m_projMatrixBufferMemoryInfos[i].pMappedData,
            &m_projMatrixData,
            sizeof(MatrixBufferObject)
        );

        VkDescriptorBufferInfo bufferInfoDesc = {
            .buffer = m_projMatrixBuffers[i],
            .offset = 0,
            .range = sizeof(MatrixBufferObject),
        };
        VkWriteDescriptorSet descriptorWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = m_projMatrixDescriptorSets[i],
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pBufferInfo = &bufferInfoDesc,
        };
        vkUpdateDescriptorSets(m_device->device(), 1, &descriptorWrite, 0, nullptr);
    }
}

void VulkanRendererImpl::createRenderPass()
{
    VkAttachmentDescription colorAttachment = {
        .format = m_surface->format(),
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    VkAttachmentReference colorAttachmentRef = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentRef
    };

    VkSubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };

    VkRenderPassCreateInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &colorAttachment,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    };

    if (vkCreateRenderPass(m_device->device(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create render pass");
    }
}

void VulkanRendererImpl::createFrameBuffers()
{
    auto swapChainImageViews = m_surface->swapChainImageViews();
    m_swapChainFramebuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++)
    {
        std::array attachments = {
            swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = m_renderPass,
            .attachmentCount = 1,
            .pAttachments = attachments.data(),
            .width = m_extent.width,
            .height = m_extent.height,
            .layers = 1
        };

        if (vkCreateFramebuffer(
            m_device->device(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]
        ) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer");
        }
    }
}

void VulkanRendererImpl::createPipeline()
{
    std::vector descriptorSetLayouts = {
        m_deviceResources->geometryDescriptorSetLayout(),
        m_projMatrixDescriptorSetLayout
    };
    std::vector pushConstantRanges = {
        VkPushConstantRange{
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
            .offset = 0,
            .size = sizeof(PushConstants)
        }
    };
    m_geometryPipeline = std::make_unique<VulkanPipeline>(
        m_device->device(), m_renderPass,
        geometry_vert_spv, geometry_vert_spv_len,
        geometry_frag_spv, geometry_frag_spv_len,
        descriptorSetLayouts, pushConstantRanges
    );

    std::vector textDescriptorSetLayouts = {
        m_deviceResources->geometryDescriptorSetLayout(),
        m_deviceResources->atlasDescriptorSetLayout(),
        m_projMatrixDescriptorSetLayout
    };
    m_textPipeline = std::make_unique<VulkanPipeline>(
        m_device->device(), m_renderPass,
        geometry_vert_spv, geometry_vert_spv_len,
        text_frag_spv, text_frag_spv_len,
        textDescriptorSetLayouts, pushConstantRanges
    );
}

void VulkanRendererImpl::doResize()
{
    vkDeviceWaitIdle(m_device->device());

    for (const auto& framebuffer : m_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(m_device->device(), framebuffer, nullptr);
    }

    m_surface->resize();
    m_extent = m_surface->extent();

    createFrameBuffers();
}
} // karin