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
}

void VkRendererImpl::endDraw()
{
}

void VkRendererImpl::resize(Size size)
{
}

void VkRendererImpl::reset()
{
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