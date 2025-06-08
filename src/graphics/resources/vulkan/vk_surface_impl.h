#ifndef SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H
#define SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H

#include <vulkan/vulkan.h>
#include <X11/Xlib.h>

#include <resources/surface_impl.h>
#include <x11/window.h>

#include "vk_graphics_device.h"

namespace karin
{

class VkSurfaceImpl : public ISurfaceImpl
{
public:
    VkSurfaceImpl(VkGraphicsDevice* device, XlibWindow window, Display* display);
    ~VkSurfaceImpl() override = default;

    void cleanUp() override;
    void present() override;
    void resize(Size size) override;
    void beforeFrame() override;

    VkViewport viewport() const;
    VkRect2D scissor() const;
    uint8_t currentFrame() const;
    VkFramebuffer currentFramebuffer() const;
    VkExtent2D extent() const;
    VkSemaphore swapChainSemaphore() const;
    VkFence swapChainFence() const;

    static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

private:
    void createSurface();
    void createSwapChain();
    void createImageView();
    void createFramebuffers();
    void createSyncObjects();
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

    std::vector<VkFramebuffer> m_swapChainFramebuffers;
    std::vector<VkSemaphore> m_swapChainSemaphores;
    std::vector<VkFence> m_swapChainFences;

    VkViewport m_viewport = {};
    VkRect2D m_scissor = {};

    uint8_t m_currentFrame = 0;
    uint32_t m_imageIndex = 0;
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_VULKAN_VK_SURFACE_IMPL_H
