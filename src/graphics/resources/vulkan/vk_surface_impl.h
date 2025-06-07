#ifndef SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H
#define SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H

#include <vulkan/vulkan.h>
#include <X11/Xlib.h>

#include <resources/surface_impl.h>

#include "vk_graphics_device.h"

namespace karin
{

class VkSurfaceImpl : public ISurfaceImpl
{
public:
    VkSurfaceImpl(VkGraphicsDevice* device, Window window, Display* display);
    ~VkSurfaceImpl() override = default;

    void cleanUp() override;
    void present() override;
    void resize(Size size) override;

private:
    void createSurface();
    void createSwapChain();
    void createImageView();

    VkGraphicsDevice* m_device;
    Window m_window;
    Display* m_display;

    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> m_swapChainImages;
    std::vector<VkImageView> m_swapChainImageViews;
    VkFormat m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
    VkExtent2D m_swapChainExtent = {};
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H
