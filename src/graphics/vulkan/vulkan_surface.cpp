#include "vulkan_surface.h"

#include <array>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <ranges>

#ifdef KARIN_PLATFORM_WINDOWS
#define NOMINMAX
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#elifdef KARIN_PLATFORM_UNIX
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>
#endif

namespace
{
VkSurfaceFormatKHR getBestSwapSurfaceFormat(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount == 0)
    {
        return {VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    std::vector<VkSurfaceFormatKHR> availableFormats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, availableFormats.data());

    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM
            && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR getBestSwapPresentMode(VkPhysicalDevice device, VkSurfaceKHR surface, bool enableVsync)
{
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount == 0)
    {
        return VK_PRESENT_MODE_FIFO_KHR; // Default present mode
    }

    std::vector<VkPresentModeKHR> availablePresentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, availablePresentModes.data());

    if (std::ranges::contains(availablePresentModes, VK_PRESENT_MODE_MAILBOX_KHR))
    {
        return VK_PRESENT_MODE_MAILBOX_KHR;
    }

    if (!enableVsync && std::ranges::contains(availablePresentModes, VK_PRESENT_MODE_IMMEDIATE_KHR))
    {
        return VK_PRESENT_MODE_IMMEDIATE_KHR;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceCapabilitiesKHR getSwapCapabilities(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);

    return capabilities;
}

VkExtent2D getSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, int width, int height)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }

    VkExtent2D actualExtent = {
        .width = static_cast<uint32_t>(width),
        .height = static_cast<uint32_t>(height),
    };

    actualExtent.width = std::clamp(
        actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width
    );
    actualExtent.height = std::clamp(
        actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height
    );

    return actualExtent;
}
}

namespace karin
{
VulkanSurface::VulkanSurface(Window::NativeHandle nativeHandle)
    : m_window(nativeHandle)
{
    createSurface();

    VulkanContext::instance().initDevices(m_surface);

    createSwapChain(false);

    createImageView();
    createViewport();
}

void VulkanSurface::cleanUp()
{
    for (auto& imageView : m_swapChainImageViews)
    {
        vkDestroyImageView(VulkanContext::instance().device(), imageView, nullptr);
    }
    m_swapChainImageViews.clear();

    if (m_swapChain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(VulkanContext::instance().device(), m_swapChain, nullptr);
        m_swapChain = VK_NULL_HANDLE;
    }

    if (m_surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(VulkanContext::instance().vkInstance(), m_surface, nullptr);
        m_surface = VK_NULL_HANDLE;
    }
}

void VulkanSurface::resize()
{
    for (auto& imageView : m_swapChainImageViews)
    {
        vkDestroyImageView(VulkanContext::instance().device(), imageView, nullptr);
    }

    createSwapChain(true);
    createImageView();
    createViewport();
}

uint32_t VulkanSurface::acquireNextImage(VkSemaphore semaphore)
{
    uint32_t imageIndex = 0;
    VkResult result = vkAcquireNextImageKHR(
        VulkanContext::instance().device(),
        m_swapChain,
        UINT64_MAX,
        semaphore,
        VK_NULL_HANDLE,
        &imageIndex
    );
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        std::cout << "[VkSurfaceManager] Swap chain out of date, resizing..." << std::endl;
        return -1;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire next image from swap chain");
    }

    return imageIndex;
}

void VulkanSurface::setViewPorts(const VkCommandBuffer commandBuffer) const
{
    vkCmdSetViewport(commandBuffer, 0, 1, &m_viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &m_scissor);
}

bool VulkanSurface::present(VkSemaphore waitSemaphore, uint32_t imageIndex) const
{
    std::array semaphores = {waitSemaphore};

    std::array swapChains = {m_swapChain};
    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = semaphores.size(),
        .pWaitSemaphores = semaphores.data(),
        .swapchainCount = static_cast<uint32_t>(swapChains.size()),
        .pSwapchains = swapChains.data(),
        .pImageIndices = &imageIndex,
        .pResults = nullptr
    };

    VkResult result = vkQueuePresentKHR(VulkanContext::instance().presentQueue(), &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        return false;
    }
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image");
    }

    return true;
}

void VulkanSurface::createSurface()
{
#ifdef KARIN_PLATFORM_WINDOWS
    VkWin32SurfaceCreateInfoKHR createInfo = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .hinstance = static_cast<HINSTANCE>(m_window.hinstance),
        .hwnd = static_cast<HWND>(m_window.hwnd)
    };

    VkResult result = vkCreateWin32SurfaceKHR(VulkanContext::instance().vkInstance(), &createInfo, nullptr, &m_surface);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create surface");
    }

#elifdef KARIN_PLATFORM_UNIX
    VkXlibSurfaceCreateInfoKHR createInfo = {
        .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .dpy = reinterpret_cast<Display*>(m_window.display),
        .window = m_window.window,
    };

    if (vkCreateXlibSurfaceKHR(VulkanContext::instance().vkInstance(), &createInfo, nullptr, &m_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create surface");
    }
#endif
}

void VulkanSurface::createSwapChain(bool isRecreating)
{
    VkSurfaceFormatKHR surfaceFormat = getBestSwapSurfaceFormat(VulkanContext::instance().physicalDevice(), m_surface);
    VkPresentModeKHR presentMode = getBestSwapPresentMode(VulkanContext::instance().physicalDevice(), m_surface, !m_isResizing);
    VkSurfaceCapabilitiesKHR capabilities = getSwapCapabilities(VulkanContext::instance().physicalDevice(), m_surface);

    VkSwapchainKHR oldSwapChain = VK_NULL_HANDLE;
    if (isRecreating)
    {
        oldSwapChain = m_swapChain;
    }

    int width, height;
#ifdef KARIN_PLATFORM_WINDOWS
    RECT rc;
    GetClientRect(reinterpret_cast<HWND>(m_window.window), &rc);
    width = rc.right - rc.left;
    height = rc.bottom - rc.top;
#elifdef KARIN_PLATFORM_UNIX
    XWindowAttributes attributes;
    XGetWindowAttributes(reinterpret_cast<Display*>(m_window.display), m_window.window, &attributes);
    width = attributes.width;
    height = attributes.height;
#endif
    VkExtent2D extent = getSwapExtent(capabilities, width, height);

    if (extent.width == 0 || extent.height == 0)
    {
        return;
    }

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
    {
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = m_surface,
        .minImageCount = imageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = presentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = oldSwapChain,
    };

    std::array queueFamilyIndices = {
        VulkanContext::instance().queueFamilyIndex(VulkanContext::QueueFamily::Graphics),
        VulkanContext::instance().queueFamilyIndex(VulkanContext::QueueFamily::Present)
    };

    if (queueFamilyIndices[0] != queueFamilyIndices[1])
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    VkSwapchainKHR swapChain;
    if (vkCreateSwapchainKHR(VulkanContext::instance().device(), &createInfo, nullptr, &swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain");
    }

    m_swapChain = swapChain;

    if (oldSwapChain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(VulkanContext::instance().device(), oldSwapChain, nullptr);
    }

    uint32_t swapChainImageCount = 0;
    vkGetSwapchainImagesKHR(VulkanContext::instance().device(), m_swapChain, &swapChainImageCount, nullptr);
    m_swapChainImages.resize(swapChainImageCount);
    vkGetSwapchainImagesKHR(VulkanContext::instance().device(), m_swapChain, &swapChainImageCount, m_swapChainImages.data());

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;
}

void VulkanSurface::createImageView()
{
    m_swapChainImageViews.resize(m_swapChainImages.size());

    for (size_t i = 0; i < m_swapChainImages.size(); i++)
    {
        VkImageViewCreateInfo viewInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = m_swapChainImages[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = m_swapChainImageFormat,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        if (vkCreateImageView(VulkanContext::instance().device(), &viewInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image views");
        }
    }
}

void VulkanSurface::createViewport()
{
    m_viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(m_swapChainExtent.width),
        .height = static_cast<float>(m_swapChainExtent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    m_scissor = {
        .offset = {0, 0},
        .extent = m_swapChainExtent
    };
}

} // karin