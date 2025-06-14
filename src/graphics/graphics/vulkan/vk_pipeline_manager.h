#ifndef SRC_GRAPHICS_GRAPHICS_VULKAN_VK_PIPELINE_MANAGER_H
#define SRC_GRAPHICS_GRAPHICS_VULKAN_VK_PIPELINE_MANAGER_H

#include <array>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace karin {

class VkPipelineManager
{
public:
    VkPipelineManager(VkDevice device, VkRenderPass renderPass);
    ~VkPipelineManager();

    void cleanUp(VkDevice device);

    VkPipeline graphicsPipeline() const;

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription = {
                .binding = 0,
                .stride = sizeof(Vertex),
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
            };
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array attributeDescriptions = {
                VkVertexInputAttributeDescription{
                    .location = 0,
                    .binding = 0,
                    .format = VK_FORMAT_R32G32_SFLOAT,
                    .offset = offsetof(Vertex, pos)
                },
                VkVertexInputAttributeDescription{
                    .location = 1,
                    .binding = 0,
                    .format = VK_FORMAT_R32G32B32_SFLOAT,
                    .offset = offsetof(Vertex, color)
                }
            };
            return attributeDescriptions;
        }
    };

private:
    void createPipeline(VkDevice device, VkRenderPass renderPass);

    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_VULKAN_VK_PIPELINE_MANAGER_H
