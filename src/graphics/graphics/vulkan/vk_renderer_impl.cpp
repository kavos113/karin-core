#include "vk_renderer_impl.h"

#include <cstring>
#include <iostream>
#include <X11/Xlib.h>

namespace karin
{
VkRendererImpl::VkRendererImpl(VkGraphicsDevice *device, Window window, Display *display)
    : m_device(device)
{
    m_surface = std::make_unique<VkSurfaceManager>(m_device, window, display);
    m_extent = m_surface->extent();

    createCommandBuffers();
    createSyncObjects();
    createRenderPass();
    createFrameBuffers();

    createVertexBuffer();
    createIndexBuffer();

    m_pipelineManager = std::make_unique<VkPipelineManager>(m_device->device(), m_renderPass);
}

void VkRendererImpl::cleanUp()
{
    vkDeviceWaitIdle(m_device->device());

    for (const auto & framebuffer : m_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(m_device->device(), framebuffer, nullptr);
    }
    m_swapChainFramebuffers.clear();

    for (const auto & semaphore : m_swapChainSemaphores)
    {
        vkDestroySemaphore(m_device->device(), semaphore, nullptr);
    }
    m_swapChainSemaphores.clear();

    for (const auto & semaphore : m_finishQueueSemaphores)
    {
        vkDestroySemaphore(m_device->device(), semaphore, nullptr);
    }
    m_finishQueueSemaphores.clear();

    for (const auto & fence : m_swapChainFences)
    {
        vkDestroyFence(m_device->device(), fence, nullptr);
    }
    m_swapChainFences.clear();

    for (auto &commandBuffer: m_commandBuffers)
    {
        vkFreeCommandBuffers(m_device->device(), m_device->commandPool(), 1, &commandBuffer);
    }
    m_commandBuffers.clear();

    vmaDestroyBuffer(m_device->allocator(), m_vertexBuffer, m_vertexAllocation);
    vmaDestroyBuffer(m_device->allocator(), m_indexBuffer, m_indexAllocation);

    m_pipelineManager->cleanUp(m_device->device());
    vkDestroyRenderPass(m_device->device(), m_renderPass, nullptr);

    m_surface->cleanUp();
}

bool VkRendererImpl::beginDraw()
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

void VkRendererImpl::endDraw()
{
    std::array vertexBuffers = {m_vertexBuffer};
    std::array<VkDeviceSize, 1> offsets = {0};
    vkCmdBindVertexBuffers(m_commandBuffers[m_currentFrame], 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());
    vkCmdBindIndexBuffer(m_commandBuffers[m_currentFrame], m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindPipeline(m_commandBuffers[m_currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineManager->graphicsPipeline());

    for (const auto & command : m_drawCommands)
    {
        m_pipelineManager->bindColorData(m_commandBuffers[m_currentFrame], command.colorData);
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

void VkRendererImpl::resize(Size size)
{
}

void VkRendererImpl::setClearColor(const Color &color)
{
    m_clearColor = {
        .color = {
            .float32 = {color.r, color.g, color.b, color.a}
        }
    };
}

void VkRendererImpl::addCommand(const std::vector<VkPipelineManager::Vertex> &vertices, std::vector<uint16_t> &indices, VkPipelineManager::ColorData colorData)
{
    memcpy(m_vertexMapPoint, vertices.data(), vertices.size() * sizeof(VkPipelineManager::Vertex));
    m_vertexMapPoint += vertices.size() * sizeof(VkPipelineManager::Vertex);

    for (uint16_t & index : indices)
    {
        index += m_vertexOffset;
    }

    memcpy(m_indexMapPoint, indices.data(), indices.size() * sizeof(uint16_t));
    m_indexMapPoint += indices.size() * sizeof(uint16_t);
    m_indexCount += indices.size();

    m_vertexOffset += static_cast<uint16_t>(vertices.size());

    m_drawCommands.push_back({
        .indexCount = static_cast<uint32_t>(indices.size()),
        .indexOffset = static_cast<uint32_t>(m_indexCount - indices.size()),
        .colorData = colorData
    });
}

Rectangle VkRendererImpl::normalize(Rectangle rect) const
{
    VkExtent2D extent = m_surface->extent();

    return {
        {
            (rect.pos.x / static_cast<float>(extent.width)) * 2.0f - 1.0f,
            (rect.pos.y / static_cast<float>(extent.height)) * 2.0f - 1.0f
        },
        {
            (rect.size.width / static_cast<float>(extent.width)) * 2.0f,
            (rect.size.height / static_cast<float>(extent.height)) * 2.0f
        }
    };
}

void VkRendererImpl::createCommandBuffers()
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

void VkRendererImpl::createSyncObjects()
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

void VkRendererImpl::createVertexBuffer()
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
    if (vmaCreateBuffer(m_device->allocator(), &bufferInfo, &allocInfo, &m_vertexBuffer, &m_vertexAllocation, &memoryInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer");
    }

    m_vertexMapPoint = memoryInfo.pMappedData;
    m_vertexStartPoint = m_vertexMapPoint;
}

void VkRendererImpl::createIndexBuffer()
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
    if (vmaCreateBuffer(m_device->allocator(), &bufferInfo, &allocInfo, &m_indexBuffer, &m_indexAllocation, &memoryInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create index buffer");
    }

    m_indexMapPoint = memoryInfo.pMappedData;
    m_indexStartPoint = m_indexMapPoint;
}

void VkRendererImpl::createRenderPass()
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

void VkRendererImpl::createFrameBuffers()
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

        if (vkCreateFramebuffer(m_device->device(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer");
        }
    }
}

void VkRendererImpl::doResize()
{
    vkDeviceWaitIdle(m_device->device());

    for (const auto & framebuffer : m_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(m_device->device(), framebuffer, nullptr);
    }

    m_surface->resize();
    m_extent = m_surface->extent();

    createFrameBuffers();
}
} // karin