#ifndef SRC_GRAPHICS_GRAPHICS_VULKAN_VK_PIPELINE_MANAGER_H
#define SRC_GRAPHICS_GRAPHICS_VULKAN_VK_PIPELINE_MANAGER_H

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>

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

    struct FragPushConstantData
    {
        glm::vec4 color;
        glm::vec3 shapeParams;
        ShapeType shapeType = ShapeType::Nothing;
    };


    void cleanUp(VkDevice device);
    void bindData(VkCommandBuffer commandBuffer, const FragPushConstantData &fragData) const;

    VkPipeline graphicsPipeline() const;

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 uv;

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
                    .format = VK_FORMAT_R32G32_SFLOAT,
                    .offset = offsetof(Vertex, uv)
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
