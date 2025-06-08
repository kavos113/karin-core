#include "vk_renderer_impl.h"

namespace karin
{
VkRendererImpl::VkRendererImpl(VkGraphicsDevice *device, VkSurfaceImpl *surface)
    : m_device(device), m_surface(surface)
{
    createCommandBuffers();
    createSemaphores();

    createVertexBuffer();
    createIndexBuffer();
}

void VkRendererImpl::cleanUp()
{
    for (auto &commandBuffer: m_commandBuffers)
    {
        vkFreeCommandBuffers(m_device->device(), m_device->commandPool(), 1, &commandBuffer);
    }
    m_commandBuffers.clear();

    for (auto &semaphore : m_finishQueueSemaphores)
    {
        vkDestroySemaphore(m_device->device(), semaphore, nullptr);
    }
    m_finishQueueSemaphores.clear();

    vmaDestroyBuffer(m_device->allocator(), m_vertexBuffer, m_vertexAllocation);
    vmaDestroyBuffer(m_device->allocator(), m_indexBuffer, m_indexAllocation);
}

void VkRendererImpl::beginDraw()
{
    uint8_t currentFrame = m_surface->currentFrame();

    vkResetCommandBuffer(m_commandBuffers[currentFrame], 0);

    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = nullptr,
    };

    if (vkBeginCommandBuffer(m_commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = m_device->renderPass(),
        .framebuffer = m_surface->currentFramebuffer(),
        .renderArea = {
            .offset = {0, 0},
            .extent = m_surface->extent(),
        },
        .clearValueCount = 1,
        .pClearValues = &m_clearColor
    };

    vkCmdBeginRenderPass(m_commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(m_commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_device->graphicsPipeline());

    std::array vertexBuffers = {m_vertexBuffer};
    std::array<VkDeviceSize, 1> offsets = {0};
    vkCmdBindVertexBuffers(m_commandBuffers[currentFrame], 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());
    vkCmdBindIndexBuffer(m_commandBuffers[currentFrame], m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    auto viewport = m_surface->viewport();
    vkCmdSetViewport(m_commandBuffers[currentFrame], 0, 1, &viewport);

    auto scissor = m_surface->scissor();
    vkCmdSetScissor(m_commandBuffers[currentFrame], 0, 1, &scissor);
}

void VkRendererImpl::endDraw()
{
    uint8_t currentFrame = m_surface->currentFrame();

    vkCmdEndRenderPass(m_commandBuffers[currentFrame]);

    if (vkEndCommandBuffer(m_commandBuffers[currentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer");
    }

    std::array semaphores = {m_surface->swapChainSemaphore()};
    std::array signalSemaphores = {m_finishQueueSemaphores[currentFrame]};
    std::array<VkPipelineStageFlags, 1> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = static_cast<uint32_t>(semaphores.size()),
        .pWaitSemaphores = semaphores.data(),
        .pWaitDstStageMask = waitStages.data(),
        .commandBufferCount = 1,
        .pCommandBuffers = &m_commandBuffers[currentFrame],
        .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()),
        .pSignalSemaphores = signalSemaphores.data(),
    };
    if (vkQueueSubmit(m_device->graphicsQueue(), 1, &submitInfo, m_surface->swapChainFence()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer");
    }
}

void VkRendererImpl::resize(Size size)
{

}

void VkRendererImpl::reset()
{
    vkDeviceWaitIdle(m_device->device());
}

void VkRendererImpl::createCommandBuffers()
{
    m_commandBuffers.resize(VkSurfaceImpl::MAX_FRAMES_IN_FLIGHT);

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

void VkRendererImpl::createSemaphores()
{
    m_finishQueueSemaphores.resize(VkSurfaceImpl::MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };

    for (auto & m_finishQueueSemaphore : m_finishQueueSemaphores)
    {
        if (vkCreateSemaphore(m_device->device(), &info, nullptr, &m_finishQueueSemaphore) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create finish queue semaphore");
        }
    }
}

void VkRendererImpl::createVertexBuffer()
{
    VmaAllocationCreateInfo allocInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };

    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = vertexBufferSize,
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (vmaCreateBuffer(m_device->allocator(), &bufferInfo, &allocInfo, &m_vertexBuffer, &m_vertexAllocation, nullptr) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer");
    }
}

void VkRendererImpl::createIndexBuffer()
{
    VmaAllocationCreateInfo allocInfo = {
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };

    VkBufferCreateInfo bufferInfo = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = indexBufferSize,
        .usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    if (vmaCreateBuffer(m_device->allocator(), &bufferInfo, &allocInfo, &m_indexBuffer, &m_indexAllocation, nullptr) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create index buffer");
    }
}
} // karin