#ifndef SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H
#define SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H

#include "vulkan_graphics_device.h"

#include <karin/system/window.h>

#include <vulkan/vulkan.h>
#include <vector>

namespace karin
{
class VulkanSurface
{
public:
    VulkanSurface(VulkanGraphicsDevice* device, Window::NativeHandle nativeHandle);
    ~VulkanSurface() = default;

    void cleanUp();
    void resize();

    uint32_t acquireNextImage(VkSemaphore semaphore);
    void setViewPorts(VkCommandBuffer commandBuffer) const;

    bool present(VkSemaphore waitSemaphore, uint32_t imageIndex) const;

    VkExtent2D extent() const
    {
        return m_swapChainExtent;
    }

    VkFormat format() const
    {
        return m_swapChainImageFormat;
    }

    uint32_t imageCount() const
    {
        return static_cast<uint32_t>(m_swapChainImages.size());
    }

    std::vector<VkImageView> swapChainImageViews() const
    {
        return m_swapChainImageViews;
    }

private:
    void createSurface();
    void createSwapChain(bool isRecreating);
    void createImageView();
    void createViewport();

    VulkanGraphicsDevice* m_device;
    Window::NativeHandle m_window;

    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> m_swapChainImages;
    std::vector<VkImageView> m_swapChainImageViews;
    VkFormat m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
    VkExtent2D m_swapChainExtent = {};

    VkViewport m_viewport = {};
    VkRect2D m_scissor = {};
};
} // karin

#endif //SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H