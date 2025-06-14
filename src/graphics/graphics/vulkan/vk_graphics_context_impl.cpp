#include "vk_graphics_context_impl.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

#include "vk_pipeline_manager.h"
#include "karin/common/color/solid_color_pattern.h"

namespace karin
{
VkGraphicsContextImpl::VkGraphicsContextImpl(VkRendererImpl *renderer)
    : m_renderer(renderer)
{
}

void VkGraphicsContextImpl::fillRect(Rectangle rect, Pattern *pattern)
{
    Rectangle normalizedRect = m_renderer->normalize(rect);

    std::vector<VkPipelineManager::Vertex> vertices = {
        {
            .pos = {normalizedRect.pos.x, normalizedRect.pos.y},
        },
        {
            .pos = {normalizedRect.pos.x + normalizedRect.size.width, normalizedRect.pos.y},
        },
        {
            .pos = {normalizedRect.pos.x + normalizedRect.size.width, normalizedRect.pos.y + normalizedRect.size.height},
        },
        {
            .pos = {normalizedRect.pos.x, normalizedRect.pos.y + normalizedRect.size.height},
        }
    };

    std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    auto *solidColorPattern = dynamic_cast<SolidColorPattern *>(pattern);
    if (!solidColorPattern)
    {
        throw std::runtime_error("VkGraphicsContextImpl::fillRect: pattern must be SolidColorPattern");
    }
    Color color = solidColorPattern->color();
    VkPipelineManager::FragPushConstantData fragData = {
        glm::vec4(color.r, color.g, color.b, color.a)
    };

    VkPipelineManager::VertPushConstantData vertData = {
        .normalize = glm::ortho(normalizedRect.pos.x, normalizedRect.pos.y,
                                normalizedRect.pos.x + normalizedRect.size.width,
                                normalizedRect.pos.y + normalizedRect.size.height)
    };

    m_renderer->addCommand(vertices, indices, vertData, fragData);
}

void VkGraphicsContextImpl::fillEllipse(Point center, float radiusX, float radiusY, Pattern *pattern)
{
}

void VkGraphicsContextImpl::fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern *pattern)
{
}

void VkGraphicsContextImpl::drawLine(Point start, Point end, Pattern *pattern, float strokeWidth)
{
}

void VkGraphicsContextImpl::drawRect(Rectangle rect, Pattern *pattern, float strokeWidth)
{
}

void VkGraphicsContextImpl::drawEllipse(Point center, float radiusX, float radiusY, Pattern *pattern, float strokeWidth)
{
}

void VkGraphicsContextImpl::drawRoundedRect(
    Rectangle rect,
    float radiusX,
    float radiusY,
    Pattern *pattern,
    float strokeWidth
)
{
}
} // karin