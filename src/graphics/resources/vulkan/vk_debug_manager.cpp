#include "vk_debug_manager.h"

#include <cstring>
#include <iostream>
#include <string>

#include "vk_debug_utils.h"
#include "vk_device_utils.h"

namespace karin
{
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
)
{
    std::string severity;
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        severity = "verbose";
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        severity = "WARNING";
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        severity = " ERROR ";
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
    {
        severity = " info  ";
    }

    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT || messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        std::cout << "Validation layer [" << severity << "] : " << pCallbackData->pMessage << std::endl;
    }
    else
    {
        std::cerr << "Validation layer [" << severity << "] : " << pCallbackData->pMessage << std::endl;
    }

    return VK_FALSE;
}

VkDebugManager::VkDebugManager(VkInstance instance)
{
    if (!VkDeviceUtils::checkValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available.");
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debugCallback,
    };
    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger");
    }
}

VkDebugManager::~VkDebugManager() = default;

void VkDebugManager::cleanup(VkInstance instance)
{
    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
}

void VkDebugManager::addDebugSupportToInstance(
    VkInstanceCreateInfo &createInfo,
    VkDebugUtilsMessengerCreateInfoEXT &debugCreateInfo
) const
{
    createInfo.enabledLayerCount = static_cast<uint32_t>(VkDeviceUtils::VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VkDeviceUtils::VALIDATION_LAYERS.data();

    debugCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debugCallback,
    };

    createInfo.pNext = &debugCreateInfo;
}

void VkDebugManager::addDebugSupportToDevice(VkDeviceCreateInfo &createInfo) const
{
    createInfo.enabledLayerCount = static_cast<uint32_t>(VkDeviceUtils::VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VkDeviceUtils::VALIDATION_LAYERS.data();
}
} // karin