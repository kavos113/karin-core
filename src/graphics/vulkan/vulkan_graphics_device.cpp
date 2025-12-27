#include "vulkan_graphics_device.h"

#include <algorithm>
#include <array>
#include <set>
#include <memory>
#include <stdexcept>
#include <vector>
#include <cstring>

#ifdef KARIN_PLATFORM_WINDOWS
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#elifdef KARIN_PLATFORM_UNIX
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>
#endif

namespace
{
const std::vector DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

bool checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    for (const auto& extension : DEVICE_EXTENSIONS)
    {
        bool extensionFound = false;

        for (const auto& availableExtension : availableExtensions)
        {
            if (strcmp(extension, availableExtension.extensionName) == 0)
            {
                extensionFound = true;
                break;
            }
        }

        if (!extensionFound)
        {
            return false;
        }
    }

    return true;
}

int rateDeviceScore(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    int score = 100;

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
    }

    if (!checkDeviceExtensionSupport(device))
    {
        return 0;
    }

    if (!deviceFeatures.samplerAnisotropy)
    {
        return 0;
    }

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    VkBool32 hasGraphicsFamily = false;

    // TODO: hasPresentFamily (requires a surface)
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueCount == 0)
        {
            continue;
        }
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            hasGraphicsFamily = true;
        }
    }

    if (!hasGraphicsFamily)
    {
        return 0;
    }

    return score;
}
}

namespace karin
{
VulkanGraphicsDevice::VulkanGraphicsDevice()
{
    createInstance();
    m_debugManager = std::make_unique<VulkanDebugManager>(m_instance);
    choosePhysicalDevice();
}

VulkanGraphicsDevice::~VulkanGraphicsDevice() = default;

void VulkanGraphicsDevice::cleanUp()
{
    vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);

    vmaDestroyAllocator(m_allocator);
    vkDestroyDevice(m_device, nullptr);
    if (m_enableValidationLayers)
    {
        m_debugManager->cleanup(m_instance);
    }
    vkDestroyInstance(m_instance, nullptr);
}

VkCommandBuffer VulkanGraphicsDevice::beginSingleTimeCommands() const
{
    VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = m_commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer commandBuffer;
    if (vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffer");
    }

    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = nullptr,
    };
    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin command buffer");
    }

    return commandBuffer;
}

void VulkanGraphicsDevice::endSingleTimeCommands(VkCommandBuffer commandBuffer) const
{
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer");
    }

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer,
    };

    if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit command buffer");
    }
    if (vkQueueWaitIdle(m_graphicsQueue) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to wait for queue idle");
    }

    vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
}

void VulkanGraphicsDevice::createInstance()
{
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Karin Application",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "Karin Library",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION
    };

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
    };

    std::vector extensions = {
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef KARIN_PLATFORM_WINDOWS
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elifdef KARIN_PLATFORM_UNIX
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#endif
    };

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (m_enableValidationLayers)
    {
        m_debugManager->addDebugSupportToInstance(createInfo, debugCreateInfo);
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create Vulkan instance");
    }
}

void VulkanGraphicsDevice::choosePhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    std::vector<int> deviceScores(deviceCount);
    for (size_t i = 0; i < devices.size(); ++i)
    {
        deviceScores[i] = rateDeviceScore(devices[i]);
    }

    auto bestDeviceIt = std::max_element(deviceScores.begin(), deviceScores.end());
    if (*bestDeviceIt == 0)
    {
        throw std::runtime_error("failed to find a suitable GPU");
    }

    m_physicalDevice = devices[std::distance(deviceScores.begin(), bestDeviceIt)];
}

// TODO: should initialize first?
void VulkanGraphicsDevice::initDevices(VkSurfaceKHR surface)
{
    if (m_device != VK_NULL_HANDLE)
    {
        return;
    }

    getQueueFamily(surface);

    createLogicalDevice();

    vkGetDeviceQueue(m_device, m_queueFamilyIndices[QueueFamily::Graphics], 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, m_queueFamilyIndices[QueueFamily::Present], 0, &m_presentQueue);

    createVmaAllocator();

    createCommandPool();
    createDescriptorPool();
}

void VulkanGraphicsDevice::getQueueFamily(VkSurfaceKHR surface)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            m_queueFamilyIndices[QueueFamily::Graphics] = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, surface, &presentSupport);
        if (presentSupport)
        {
            m_queueFamilyIndices[QueueFamily::Present] = i;
        }

        i++;
    }
}

void VulkanGraphicsDevice::createVmaAllocator()
{
    VmaAllocatorCreateInfo allocatorInfo = {
        .flags = 0,
        .physicalDevice = m_physicalDevice,
        .device = m_device,
        .instance = m_instance,
        .vulkanApiVersion = VK_API_VERSION
    };

    if (vmaCreateAllocator(&allocatorInfo, &m_allocator) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create VMA allocator");
    }
}

void VulkanGraphicsDevice::createLogicalDevice()
{
    std::set uniqueQueueFamilies = {
        m_queueFamilyIndices[QueueFamily::Graphics],
        m_queueFamilyIndices[QueueFamily::Present]
    };
    float queuePriority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queueFamily,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority,
        };
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {
        .sampleRateShading = VK_TRUE,
        .samplerAnisotropy = VK_TRUE,
    };

    VkDeviceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size()),
        .ppEnabledExtensionNames = DEVICE_EXTENSIONS.data(),
        .pEnabledFeatures = &deviceFeatures,
    };

    if (m_enableValidationLayers)
    {
        m_debugManager->addDebugSupportToDevice(createInfo);
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device");
    }
}

void VulkanGraphicsDevice::createCommandPool()
{
    VkCommandPoolCreateInfo poolInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = m_queueFamilyIndices[QueueFamily::Graphics],
    };

    if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool");
    }
}

void VulkanGraphicsDevice::createDescriptorPool()
{
    std::array sizes = {
        VkDescriptorPoolSize{
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 2048,
        },
        VkDescriptorPoolSize{
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 2048,
        }
    };

    VkDescriptorPoolCreateInfo poolInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = 4096,
        .poolSizeCount = static_cast<uint32_t>(sizes.size()),
        .pPoolSizes = sizes.data(),
    };

    if (vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool");
    }
}
} // karin