#include "vk_graphics_device.h"

#include <vulkan/vulkan_xlib.h>
#include <algorithm>
#include <array>

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

void VkGraphicsDevice::initDevices(VkSurfaceKHR surface)
{
    if (m_device != VK_NULL_HANDLE)
    {
        return;
    }

    getQueueFamily(surface);

    createLogicalDevice(surface);

    vkGetDeviceQueue(m_device, m_queueFamilyIndices[QueueFamily::Graphics], 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, m_queueFamilyIndices[QueueFamily::Present], 0, &m_presentQueue);

    createCommandPool();
    createRenderPass();
    createPipeline();
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

void VkGraphicsDevice::createLogicalDevice(VkSurfaceKHR surface)
{
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

void VkGraphicsDevice::createCommandPool()
{
    VkCommandPoolCreateInfo poolInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .queueFamilyIndex = m_queueFamilyIndices[QueueFamily::Graphics],
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
    };

    if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool");
    }
}

void VkGraphicsDevice::createRenderPass()
{
    VkAttachmentDescription colorAttachment = {
        .format = VK_FORMAT_B8G8R8A8_SRGB, // Assuming SRGB format for swapchain
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    VkAttachmentReference colorAttachmentRef = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentRef
    };

    VkSubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };

    VkRenderPassCreateInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &colorAttachment,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &dependency
    };

    if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create render pass");
    }
}

void VkGraphicsDevice::createPipeline()
{
    auto vertShader = VkDeviceUtils::loadShader(m_device, "./shaders/vert.spv");
    auto fragShader = VkDeviceUtils::loadShader(m_device, "./shaders/frag.spv");

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertShader,
        .pName = "main"
    };

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragShader,
        .pName = "main"
    };

    std::array shaderStages = {
        vertShaderStageInfo,
        fragShaderStageInfo
    };

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = &bindingDescription,
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
        .pVertexAttributeDescriptions = attributeDescriptions.data()
    };

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    std::array dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
        .pDynamicStates = dynamicStates.data()
    };

    VkPipelineViewportStateCreateInfo viewportState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .scissorCount = 1
    };

    VkPipelineRasterizationStateCreateInfo rasterizerInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .lineWidth = 1.0f,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f
    };

    VkPipelineMultisampleStateCreateInfo multisampling = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .sampleShadingEnable = VK_FALSE,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .minSampleShading = 1.0f,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE
    };

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {
        .blendEnable = VK_FALSE,
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    VkPipelineColorBlendStateCreateInfo colorBlending = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &colorBlendAttachment
    };

    VkPipelineLayoutCreateInfo layoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr
    };
    if (vkCreatePipelineLayout(m_device, &layoutCreateInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = shaderStages.data(),

        .pVertexInputState = &vertexInputInfo,
        .pInputAssemblyState = &inputAssembly,
        .pViewportState = &viewportState,
        .pRasterizationState = &rasterizerInfo,
        .pMultisampleState = &multisampling,
        .pDepthStencilState = nullptr,
        .pColorBlendState = &colorBlending,
        .pDynamicState = &dynamicState,

        .layout = m_pipelineLayout,
        .renderPass = m_renderPass,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = -1
    };
    if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline");
    }

    vkDestroyShaderModule(m_device, vertShader, nullptr);
    vkDestroyShaderModule(m_device, fragShader, nullptr);
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

VkRenderPass VkGraphicsDevice::renderPass() const
{
    return m_renderPass;
}

VkCommandPool VkGraphicsDevice::commandPool() const
{
    return m_commandPool;
}

VkPipeline VkGraphicsDevice::graphicsPipeline() const
{
    return m_graphicsPipeline;
}

VkQueue VkGraphicsDevice::graphicsQueue() const
{
    return m_graphicsQueue;
}

VkQueue VkGraphicsDevice::presentQueue() const
{
    return m_presentQueue;
}
} // karin