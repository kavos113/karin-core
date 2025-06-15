#include "vk_graphics_context_impl.h"

#include <iostream>
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
            .uv = {-1.0f, -1.0f},
        },
        {
            .pos = {normalizedRect.pos.x + normalizedRect.size.width, normalizedRect.pos.y},
            .uv = {1.0f, -1.0f},
        },
        {
            .pos = {normalizedRect.pos.x + normalizedRect.size.width, normalizedRect.pos.y + normalizedRect.size.height},
            .uv = {1.0f, 1.0f},
        },
        {
            .pos = {normalizedRect.pos.x, normalizedRect.pos.y + normalizedRect.size.height},
            .uv = {-1.0f, 1.0f},
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

    m_renderer->addCommand(vertices, indices, fragData);
}

void VkGraphicsContextImpl::fillEllipse(Point center, float radiusX, float radiusY, Pattern *pattern)
{
    Rectangle rect = m_renderer->normalize(Rectangle(
        center.x - radiusX,
        center.y - radiusY,
        radiusX * 2.0f,
        radiusY * 2.0f
    ));

    std::vector<VkPipelineManager::Vertex> vertices = {
        {
            .pos = {rect.pos.x, rect.pos.y},
            .uv = {-1.0f, -1.0f},
        },
        {
            .pos = {rect.pos.x + rect.size.width, rect.pos.y},
            .uv = {1.0f, -1.0f},
        },
        {
            .pos = {rect.pos.x + rect.size.width, rect.pos.y + rect.size.height},
            .uv = {1.0f, 1.0f},
        },
        {
            .pos = {rect.pos.x, rect.pos.y + rect.size.height},
            .uv = {-1.0f, 1.0f},
        }
    };

    std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    auto *solidColorPattern = dynamic_cast<SolidColorPattern *>(pattern);
    if (!solidColorPattern)
    {
        throw std::runtime_error("VkGraphicsContextImpl::fillEllipse: pattern must be SolidColorPattern");
    }
    Color color = solidColorPattern->color();
    VkPipelineManager::FragPushConstantData fragData = {
        .color = glm::vec4(color.r, color.g, color.b, color.a),
        .shapeType = VkPipelineManager::ShapeType::Ellipse,
    };

    m_renderer->addCommand(vertices, indices, fragData);
}

void VkGraphicsContextImpl::fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern *pattern)
{
    Rectangle normalizedRect = m_renderer->normalize(rect);

    std::vector<VkPipelineManager::Vertex> vertices = {
        {
            .pos = {normalizedRect.pos.x, normalizedRect.pos.y},
            .uv = {-1.0f, -1.0f},
        },
        {
            .pos = {normalizedRect.pos.x + normalizedRect.size.width, normalizedRect.pos.y},
            .uv = {1.0f, -1.0f},
        },
        {
            .pos = {normalizedRect.pos.x + normalizedRect.size.width, normalizedRect.pos.y + normalizedRect.size.height},
            .uv = {1.0f, 1.0f},
        },
        {
            .pos = {normalizedRect.pos.x, normalizedRect.pos.y + normalizedRect.size.height},
            .uv = {-1.0f, 1.0f},
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
        .color = glm::vec4(color.r, color.g, color.b, color.a),
        .shapeParams = glm::vec3(radiusX / rect.size.width * 2.0f, radiusY / rect.size.height * 2.0f, 0.0f),
        .shapeType = VkPipelineManager::ShapeType::RoundedRectangle,
    };

    m_renderer->addCommand(vertices, indices, fragData);
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