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

    enum class ShapeType : uint32_t
    {
        Nothing = 0,
        Ellipse = 1,
        RoundedRectangle = 2,
    };

    struct VertPushConstantData
    {
        glm::mat4 normalize;
    };

    struct FragPushConstantData
    {
        glm::vec4 color;
        glm::vec3 shapeParams;
        ShapeType shapeType = ShapeType::Nothing;
    };


    void cleanUp(VkDevice device);
    void bindData(VkCommandBuffer commandBuffer, FragPushConstantData fragData, VertPushConstantData vertData) const;

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
