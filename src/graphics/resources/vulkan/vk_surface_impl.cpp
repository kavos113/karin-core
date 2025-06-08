#include "vk_surface_impl.h"

#include <vulkan/vulkan_xlib.h>
#include <array>

#include "vk_device_utils.h"

namespace karin
{
VkSurfaceImpl::VkSurfaceImpl(VkGraphicsDevice *device, Window window, Display *display)
    : m_device(device), m_window(window), m_display(display)
{
    createSurface();
    createSwapChain();
    createImageView();
    createFramebuffers();
    createSyncObjects();
}

void VkSurfaceImpl::cleanUp()
{
    for (auto &imageView : m_swapChainImageViews)
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

void VkSurfaceImpl::present()
{
    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = nullptr,
        .swapchainCount = 1,
        .pSwapchains = &m_swapChain,
        .pImageIndices = nullptr,
        .pResults = nullptr
    };
}

void VkSurfaceImpl::resize(Size size)
{
}

void VkSurfaceImpl::createSurface()
{
    VkXlibSurfaceCreateInfoKHR createInfo = {
        .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        .dpy = m_display,
        .window = m_window,
    };

    if (vkCreateXlibSurfaceKHR(m_device->instance(), &createInfo, nullptr, &m_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create surface");
    }

    m_device->initDevices(m_surface);
}

void VkSurfaceImpl::createSwapChain()
{
    VkSurfaceFormatKHR surfaceFormat = VkDeviceUtils::getBestSwapSurfaceFormat(m_device->physicalDevice(), m_surface);
    VkPresentModeKHR presentMode = VkDeviceUtils::getBestSwapPresentMode(m_device->physicalDevice(), m_surface);
    VkSurfaceCapabilitiesKHR capabilities = VkDeviceUtils::getSwapCapabilities(m_device->physicalDevice(), m_surface);

    XWindowAttributes attributes;
    XGetWindowAttributes(m_display, m_window, &attributes);
    VkExtent2D extent = VkDeviceUtils::getSwapExtent(capabilities, attributes.width, attributes.height);

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
        m_device->queueFamilyIndex(VkGraphicsDevice::QueueFamily::Graphics),
        m_device->queueFamilyIndex(VkGraphicsDevice::QueueFamily::Present)
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

void VkSurfaceImpl::createImageView()
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

void VkSurfaceImpl::createFramebuffers()
{
    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

    for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
    {
        std::array attachments = {
            m_swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = m_device->renderPass(),
            .attachmentCount = 1,
            .pAttachments = attachments.data(),
            .width = m_swapChainExtent.width,
            .height = m_swapChainExtent.height,
            .layers = 1
        };

        if (vkCreateFramebuffer(m_device->device(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer");
        }
    }
}

void VkSurfaceImpl::createSyncObjects()
{
    m_swapchainSemaphores.resize(m_swapChainImages.size());
    m_swapchainFences.resize(m_swapChainImages.size());

    VkSemaphoreCreateInfo semaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    VkFenceCreateInfo fenceInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    for (size_t i = 0; i < m_swapChainImages.size(); i++)
    {
        if (vkCreateSemaphore(m_device->device(), &semaphoreInfo, nullptr, &m_swapchainSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(m_device->device(), &fenceInfo, nullptr, &m_swapchainFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain sync objects");
        }
    }
}
} // karin