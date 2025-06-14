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

    struct ColorData
    {
        glm::vec4 color;
    };


    void cleanUp(VkDevice device);
    void bindColorData(VkCommandBuffer commandBuffer, ColorData colorData) const;

    VkPipeline graphicsPipeline() const;

    struct Vertex
    {
        glm::vec2 pos;

        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription = {
                .binding = 0,
                .stride = sizeof(Vertex),
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
            };
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions()
        {
            std::array attributeDescriptions = {
                VkVertexInputAttributeDescription{
                    .location = 0,
                    .binding = 0,
                    .format = VK_FORMAT_R32G32_SFLOAT,
                    .offset = offsetof(Vertex, pos)
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
