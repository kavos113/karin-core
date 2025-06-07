#include "vk_graphics_device.h"

#include <vulkan/vulkan_xlib.h>
#include <algorithm>

#include "vk_device_utils.h"

namespace karin
{
VkGraphicsDevice::VkGraphicsDevice()
{
    createInstance();
    m_debugManager = std::make_unique<VkDebugManager>(m_instance);
    choosePhysicalDevice();
    createVmaAllocator();
}

VkGraphicsDevice::~VkGraphicsDevice()
{
}

void VkGraphicsDevice::cleanUp()
{
    vmaDestroyAllocator(m_allocator);
    vkDestroyDevice(m_device, nullptr);
    if (m_enableValidationLayers)
    {
        m_debugManager->cleanup(m_instance);
    }
    vkDestroyInstance(m_instance, nullptr);
}

void VkGraphicsDevice::createInstance()
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
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME
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

void VkGraphicsDevice::choosePhysicalDevice()
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
        deviceScores[i] = VkDeviceUtils::rateDeviceScore(devices[i]);
    }

    auto bestDeviceIt = std::max_element(deviceScores.begin(), deviceScores.end());
    if (*bestDeviceIt == 0)
    {
        throw std::runtime_error("failed to find a suitable GPU");
    }

    m_physicalDevice = devices[std::distance(deviceScores.begin(), bestDeviceIt)];
}

void VkGraphicsDevice::createLogicalDevice(VkSurfaceKHR surface)
{
    if (m_device != VK_NULL_HANDLE)
    {
        return;
    }

    getQueueFamily(surface);

    float queuePriority = 1.0f;
    std::vector queueCreateInfos = {
        VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = m_queueFamilyIndices[QueueFamily::Graphics],
            .queueCount = 1,
            .pQueuePriorities = &queuePriority
        },
        VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = m_queueFamilyIndices[QueueFamily::Present],
            .queueCount = 1,
            .pQueuePriorities = &queuePriority
        }
    };

    VkPhysicalDeviceFeatures deviceFeatures = {
        .samplerAnisotropy = VK_TRUE,
        .sampleRateShading = VK_TRUE
    };

    VkDeviceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pQueueCreateInfos = queueCreateInfos.data(),
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
        .pEnabledFeatures = &deviceFeatures,
        .enabledExtensionCount = static_cast<uint32_t>(VkDeviceUtils::DEVICE_EXTENSIONS.size()),
        .ppEnabledExtensionNames = VkDeviceUtils::DEVICE_EXTENSIONS.data()
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

void VkGraphicsDevice::getQueueFamily(VkSurfaceKHR surface)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto & queueFamily : queueFamilies)
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

void VkGraphicsDevice::createVmaAllocator()
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

VkInstance VkGraphicsDevice::instance() const
{
    return m_instance;
}

VkPhysicalDevice VkGraphicsDevice::physicalDevice() const
{
    return m_physicalDevice;
}

VkDevice VkGraphicsDevice::device() const
{
    return m_device;
}

VmaAllocator VkGraphicsDevice::allocator() const
{
    return m_allocator;
}

uint32_t VkGraphicsDevice::queueFamilyIndex(QueueFamily family) const
{
    return m_queueFamilyIndices.at(family);
}
} // karin