#include "vulkan_surface.h"

#include <array>
#include <iostream>

#include "vulkan_utils.h"


#ifdef KARIN_PLATFORM_WINDOWS
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#elifdef KARIN_PLATFORM_UNIX
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>
#endif

namespace karin
{
VulkanSurface::VulkanSurface(VulkanGraphicsDevice* device, Window::NativeHandle nativeHandle)
    : m_device(device), m_window(nativeHandle)
{
    createSurface();

    m_device->initDevices(m_surface);

    createSwapChain();

    createImageView();
    createViewport();
}

void VulkanSurface::cleanUp()
{
    for (auto& imageView : m_swapChainImageViews)
    {
        vkDestroyImageView(m_device->device(), imageView, nullptr);
    }
    m_swapChainImageViews.clear();

    if (m_swapChain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(m_device->device(), m_swapChain, nullptr);
        m_swapChain = VK_NULL_HANDLE;
    }

    if (m_surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(m_device->instance(), m_surface, nullptr);
        m_surface = VK_NULL_HANDLE;
    }
}

void VulkanSurface::resize()
{
    for (auto& imageView : m_swapChainImageViews)
    {
        vkDestroyImageView(m_device->device(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(m_device->device(), m_swapChain, nullptr);

    createSwapChain();
    createImageView();
    createViewport();
}

uint32_t VulkanSurface::acquireNextImage(VkSemaphore semaphore)
{
    uint32_t imageIndex = 0;
    VkResult result = vkAcquireNextImageKHR(
        m_device->device(),
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

    VkResult result = vkQueuePresentKHR(m_device->presentQueue(), &presentInfo);
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

    VkResult result = vkCreateWin32SurfaceKHR(m_device->instance(), &createInfo, nullptr, &m_surface);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create surface");
    }

#elifdef KARIN_PLATFORM_UNIX
    VkXlibSurfaceCreateInfoKHR createInfo = {
        .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .dpy = reinterpret_cast<Display *>(m_window.display),
        .window = m_window.window,
    };

    if (vkCreateXlibSurfaceKHR(m_device->instance(), &createInfo, nullptr, &m_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create surface");
    }
#endif
}

void VulkanSurface::createSwapChain()
{
    VkSurfaceFormatKHR surfaceFormat = VulkanUtils::getBestSwapSurfaceFormat(m_device->physicalDevice(), m_surface);
    VkPresentModeKHR presentMode = VulkanUtils::getBestSwapPresentMode(m_device->physicalDevice(), m_surface);
    VkSurfaceCapabilitiesKHR capabilities = VulkanUtils::getSwapCapabilities(m_device->physicalDevice(), m_surface);

    int width, height;
#ifdef KARIN_PLATFORM_WINDOWS
    RECT rc;
    GetClientRect(reinterpret_cast<HWND>(m_window.window), &rc);
    width = rc.right - rc.left;
    height = rc.bottom - rc.top;
#elifdef KARIN_PLATFORM_UNIX
    XWindowAttributes attributes;
    XGetWindowAttributes(reinterpret_cast<Display *>(m_window.display), m_window.window, &attributes);
    width = attributes.width;
    height = attributes.height;
#endif
    VkExtent2D extent = VulkanUtils::getSwapExtent(capabilities, width, height);

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
        .oldSwapchain = VK_NULL_HANDLE,
    };

    std::array queueFamilyIndices = {
        m_device->queueFamilyIndex(VulkanGraphicsDevice::QueueFamily::Graphics),
        m_device->queueFamilyIndex(VulkanGraphicsDevice::QueueFamily::Present)
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

    if (vkCreateSwapchainKHR(m_device->device(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain");
    }

    uint32_t swapChainImageCount = 0;
    vkGetSwapchainImagesKHR(m_device->device(), m_swapChain, &swapChainImageCount, nullptr);
    m_swapChainImages.resize(swapChainImageCount);
    vkGetSwapchainImagesKHR(m_device->device(), m_swapChain, &swapChainImageCount, m_swapChainImages.data());

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

        if (vkCreateImageView(m_device->device(), &viewInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
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

VkExtent2D VulkanSurface::extent() const
{
    return m_swapChainExtent;
}

std::vector<VkImageView> VulkanSurface::swapChainImageViews() const
{
    return m_swapChainImageViews;
}

VkFormat VulkanSurface::format() const
{
    return m_swapChainImageFormat;
}

uint32_t VulkanSurface::imageCount() const
{
    return static_cast<uint32_t>(m_swapChainImages.size());
}
} // karin
