#ifndef SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H
#define SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H

#include <vulkan/vulkan.h>
#include <X11/Xlib.h>

#include <x11/window.h>

#include "vk_graphics_device.h"

namespace karin
{

class VkSurfaceManager
{
public:
    VkSurfaceManager(VkGraphicsDevice* device, XlibWindow window, Display* display);
    ~VkSurfaceManager() = default;

    void cleanUp();
    void resize();

    uint32_t acquireNextImage(VkSemaphore semaphore);
    void setViewPorts(VkCommandBuffer commandBuffer) const;

    bool present(VkSemaphore waitSemaphore, uint32_t imageIndex) const;

    VkExtent2D extent() const;
    VkFormat format() const;
    uint32_t imageCount() const;

    std::vector<VkImageView> swapChainImageViews() const;

private:
    void createSurface();
    void createSwapChain();
    void createImageView();
    void createViewport();

    VkGraphicsDevice* m_device;
    Window m_window;
    Display* m_display;

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
