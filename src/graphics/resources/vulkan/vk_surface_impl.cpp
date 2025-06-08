#include "vk_surface_impl.h"

#include <vulkan/vulkan_xlib.h>
#include <array>

#include "vk_device_utils.h"

namespace karin
{
VkSurfaceImpl::VkSurfaceImpl(VkGraphicsDevice *device, XlibWindow window, Display *display)
    : m_device(device), m_window(window), m_display(display)
{
    createSurface();
    createSwapChain();
    createImageView();
    createFramebuffers();
    createSyncObjects();
    createViewport();
}

void VkSurfaceImpl::cleanUp()
{
    for (auto &semafore : m_swapChainSemaphores)
    {
        vkDestroySemaphore(m_device->device(), semafore, nullptr);
    }

    for (auto &fence : m_swapChainFences)
    {
        vkDestroyFence(m_device->device(), fence, nullptr);
    }

    for (auto &framebuffer : m_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(m_device->device(), framebuffer, nullptr);
    }

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
    std::array swapChains = { m_swapChain };
    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_swapChainSemaphores[m_currentFrame],
        .swapchainCount = static_cast<uint32_t>(swapChains.size()),
        .pSwapchains = swapChains.data(),
        .pImageIndices = &m_imageIndex,
        .pResults = nullptr
    };

    VkResult result = vkQueuePresentKHR(m_device->presentQueue(), &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        // recreate swapchain
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image");
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VkSurfaceImpl::resize(Size size)
{
    for (auto &framebuffer : m_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(m_device->device(), framebuffer, nullptr);
    }

    for (auto &imageView : m_swapChainImageViews)
    {
        vkDestroyImageView(m_device->device(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(m_device->device(), m_swapChain, nullptr);

    createSwapChain();
    createImageView();
    createFramebuffers();
}

void VkSurfaceImpl::beforeFrame()
{
    vkWaitForFences(m_device->device(), 1, &m_swapChainFences[m_currentFrame], VK_TRUE, UINT64_MAX);

    VkResult result = vkAcquireNextImageKHR(
        m_device->device(),
        m_swapChain,
        UINT64_MAX,
        m_swapChainSemaphores[m_currentFrame],
        VK_NULL_HANDLE,
        &m_imageIndex
    );
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        // recreate swapchain
        return;
    }
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to acquire swap chain image");
    }

    vkResetFences(m_device->device(), 1, &m_swapChainFences[m_currentFrame]);
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
    m_swapChainSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
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
}

void VkSurfaceImpl::createViewport()
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

VkViewport VkSurfaceImpl::viewport() const
{
    return m_viewport;
}

VkRect2D VkSurfaceImpl::scissor() const
{
    return m_scissor;
}

uint8_t VkSurfaceImpl::currentFrame() const
{
    return m_currentFrame;
}

VkFramebuffer VkSurfaceImpl::currentFramebuffer() const
{
    return m_swapChainFramebuffers[m_currentFrame];
}

VkExtent2D VkSurfaceImpl::extent() const
{
    return m_swapChainExtent;
}

VkSemaphore VkSurfaceImpl::swapChainSemaphore() const
{
    return m_swapChainSemaphores[m_currentFrame];
}

VkFence VkSurfaceImpl::swapChainFence() const
{
    return m_swapChainFences[m_currentFrame];
}
} // karin