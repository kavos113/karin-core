#include "vulkan_graphics_context_impl.h"

#include "vulkan_renderer_impl.h"
#include "glm_geometry.h"

#include <karin/common/color/color.h>
#include <karin/common/color/pattern.h>
#include <karin/common/color/solid_color_pattern.h>
#include <karin/common/geometry/point.h>
#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/stroke_style.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numbers>

namespace karin
{
VulkanGraphicsContextImpl::VulkanGraphicsContextImpl(VulkanRendererImpl *renderer)
    : m_renderer(renderer)
{
}

void VulkanGraphicsContextImpl::fillRect(Rectangle rect, Pattern *pattern)
{
    Rectangle normalizedRect = m_renderer->normalize(rect);

    std::vector<VulkanPipeline::Vertex> vertices = {
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
    VulkanPipeline::FragPushConstantData fragData = {
        glm::vec4(color.r, color.g, color.b, color.a)
    };

    m_renderer->addCommand(vertices, indices, fragData);
}

void VulkanGraphicsContextImpl::fillEllipse(Point center, float radiusX, float radiusY, Pattern *pattern)
{
    Rectangle rect = m_renderer->normalize(Rectangle(
        center.x - radiusX,
        center.y - radiusY,
        radiusX * 2.0f,
        radiusY * 2.0f
    ));

    std::vector<VulkanPipeline::Vertex> vertices = {
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
    VulkanPipeline::FragPushConstantData fragData = {
        .color = glm::vec4(color.r, color.g, color.b, color.a),
        .shapeType = VulkanPipeline::ShapeType::Ellipse,
    };

    m_renderer->addCommand(vertices, indices, fragData);
}

void VulkanGraphicsContextImpl::fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern *pattern)
{
    Rectangle normalizedRect = m_renderer->normalize(rect);

    std::vector<VulkanPipeline::Vertex> vertices = {
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
    VulkanPipeline::FragPushConstantData fragData = {
        .color = glm::vec4(color.r, color.g, color.b, color.a),
        .shapeParams = glm::vec3(radiusX / rect.size.width * 2.0f, radiusY / rect.size.height * 2.0f, 0.0f),
        .shapeType = VulkanPipeline::ShapeType::RoundedRectangle,
    };

    m_renderer->addCommand(vertices, indices, fragData);
}

void VulkanGraphicsContextImpl::drawLine(Point start, Point end, Pattern *pattern, const StrokeStyle& strokeStyle)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    addLine(start, end, strokeStyle, vertices, indices);

    auto *solidColorPattern = dynamic_cast<SolidColorPattern *>(pattern);
    if (!solidColorPattern)
    {
        throw std::runtime_error("VkGraphicsContextImpl::drawLine: pattern must be SolidColorPattern");
    }
    Color color = solidColorPattern->color();
    VulkanPipeline::FragPushConstantData fragData = {
        .color = glm::vec4(color.r, color.g, color.b, color.a),
    };

    m_renderer->addCommand(vertices, indices, fragData);
}

void VulkanGraphicsContextImpl::drawRect(Rectangle rect, Pattern *pattern, const StrokeStyle& strokeStyle)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    StrokeStyle style = strokeStyle;
    style.start_cap_style = style.dash_cap_style;
    style.end_cap_style = style.dash_cap_style;
    float dashOffset = addLine(
        Point(rect.pos.x, rect.pos.y),
        Point(rect.pos.x + rect.size.width, rect.pos.y),
        style,
        vertices,
        indices
    );
    style.dash_offset = dashOffset;
    dashOffset = addLine(
        Point(rect.pos.x + rect.size.width, rect.pos.y),
        Point(rect.pos.x + rect.size.width, rect.pos.y + rect.size.height),
        style,
        vertices,
        indices
    );
    style.dash_offset = dashOffset;
    dashOffset = addLine(
        Point(rect.pos.x + rect.size.width, rect.pos.y + rect.size.height),
        Point(rect.pos.x, rect.pos.y + rect.size.height),
        style,
        vertices,
        indices
    );
    style.dash_offset = dashOffset;
    addLine(
        Point(rect.pos.x, rect.pos.y + rect.size.height),
        Point(rect.pos.x, rect.pos.y),
        style,
        vertices,
        indices
    );

    auto *solidColorPattern = dynamic_cast<SolidColorPattern *>(pattern);
    if (!solidColorPattern)
    {
        throw std::runtime_error("VkGraphicsContextImpl::drawRect: pattern must be SolidColorPattern");
    }
    Color color = solidColorPattern->color();
    VulkanPipeline::FragPushConstantData fragData = {
        .color = glm::vec4(color.r, color.g, color.b, color.a),
    };

    m_renderer->addCommand(vertices, indices, fragData);
}

void VulkanGraphicsContextImpl::drawEllipse(Point center, float radiusX, float radiusY, Pattern *pattern, const StrokeStyle& strokeStyle)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    StrokeStyle style = strokeStyle;
    style.start_cap_style = style.dash_cap_style;
    style.end_cap_style = style.dash_cap_style;

    addArc(
        center,
        radiusX,
        radiusY,
        0.0f,
        2.0f * std::numbers::pi,
        style,
        vertices,
        indices
    );

    auto *solidColorPattern = dynamic_cast<SolidColorPattern *>(pattern);
    if (!solidColorPattern)
    {
        throw std::runtime_error("VkGraphicsContextImpl::drawEllipse: pattern must be SolidColorPattern");
    }
    Color color = solidColorPattern->color();
    VulkanPipeline::FragPushConstantData fragData = {
        .color = glm::vec4(color.r, color.g, color.b, color.a),
    };
    m_renderer->addCommand(vertices, indices, fragData);
}

void VulkanGraphicsContextImpl::drawRoundedRect(
    Rectangle rect,
    float radiusX,
    float radiusY,
    Pattern *pattern,
    const StrokeStyle& strokeStyle
)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    StrokeStyle style = strokeStyle;
    style.start_cap_style = style.dash_cap_style;
    style.end_cap_style = style.dash_cap_style;

    float offset = addArc(
        Point(rect.pos.x + radiusX, rect.pos.y + radiusY),
        radiusX,
        radiusY,
        std::numbers::pi,
        1.5f * std::numbers::pi,
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = addLine(
        Point(rect.pos.x + radiusX, rect.pos.y),
        Point(rect.pos.x + rect.size.width - radiusX, rect.pos.y),
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = addArc(
        Point(rect.pos.x + rect.size.width - radiusX, rect.pos.y + radiusY),
        radiusX,
        radiusY,
        1.5f * std::numbers::pi,
        2.0f * std::numbers::pi,
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = addLine(
        Point(rect.pos.x + rect.size.width, rect.pos.y + radiusY),
        Point(rect.pos.x + rect.size.width, rect.pos.y + rect.size.height - radiusY),
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = addArc(
        Point(rect.pos.x + rect.size.width - radiusX, rect.pos.y + rect.size.height - radiusY),
        radiusX,
        radiusY,
        0.0f,
        0.5f * std::numbers::pi,
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = addLine(
        Point(rect.pos.x + rect.size.width - radiusX, rect.pos.y + rect.size.height),
        Point(rect.pos.x + radiusX, rect.pos.y + rect.size.height),
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = addArc(
        Point(rect.pos.x + radiusX, rect.pos.y + rect.size.height - radiusY),
        radiusX,
        radiusY,
        0.5f * std::numbers::pi,
        std::numbers::pi,
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    addLine(
        Point(rect.pos.x, rect.pos.y + rect.size.height - radiusY),
        Point(rect.pos.x, rect.pos.y + radiusY),
        style,
        vertices,
        indices
    );

    auto *solidColorPattern = dynamic_cast<SolidColorPattern *>(pattern);
    if (!solidColorPattern)
    {
        throw std::runtime_error("VkGraphicsContextImpl::drawRoundedRect: pattern must be SolidColorPattern");
    }
    Color color = solidColorPattern->color();
    VulkanPipeline::FragPushConstantData fragData = {
        .color = glm::vec4(color.r, color.g, color.b, color.a),
    };
    m_renderer->addCommand(vertices, indices, fragData);
}

void VulkanGraphicsContextImpl::fillPath(const Path& path, Pattern* pattern)
{
}

void VulkanGraphicsContextImpl::drawPath(const Path& path, Pattern* pattern, const StrokeStyle& strokeStyle)
{
}

float VulkanGraphicsContextImpl::addLine(
    Point start,
    Point end,
    const StrokeStyle &strokeStyle,
    std::vector<VulkanPipeline::Vertex> &vertices,
    std::vector<uint16_t> &indices
) const
{
    float dashOffset = 0.0f;

    auto startVec = toGlmVec2(m_renderer->normalize(start));
    auto endVec = toGlmVec2(m_renderer->normalize(end));

    auto direction = end - start;
    auto normalVec = glm::normalize(glm::vec2(-direction.y, direction.x)) * strokeStyle.width / 2.0f;

    auto normalUnitVec = m_renderer->normalizeVec(normalVec);
    auto dirUnitVec = m_renderer->normalizeVec(glm::normalize(toGlmVec2(direction)) * strokeStyle.width);

    std::vector<std::pair<glm::vec2, glm::vec2>> lines;
    auto current = startVec - dirUnitVec * strokeStyle.dash_offset;
    if (strokeStyle.dash_pattern.empty())
    {
        lines.push_back({startVec, endVec});
    }
    else
    {
        auto dashPatternIndex = 0;
        while (true)
        {
            std::pair<glm::vec2, glm::vec2> line;
            line.first = current;
            if ((endVec.x - current.x) / dirUnitVec.x < strokeStyle.dash_pattern[dashPatternIndex]
                || (endVec.y - current.y) / dirUnitVec.y < strokeStyle.dash_pattern[dashPatternIndex])
            {
                line.second = endVec;
                lines.push_back(line);
                if (dirUnitVec.x != 0.0f)
                {
                    dashOffset = (endVec.x - current.x) / dirUnitVec.x;
                }
                else if (dirUnitVec.y != 0.0f)
                {
                    dashOffset = (endVec.y - current.y) / dirUnitVec.y;
                }
                break;
            }

            line.second = current + dirUnitVec * strokeStyle.dash_pattern[dashPatternIndex];
            lines.push_back(line);

            current = line.second;
            dashPatternIndex = (dashPatternIndex + 1) % strokeStyle.dash_pattern.size();
        }

        lines[0].first += dirUnitVec * strokeStyle.dash_offset;
    }

    for (int i = 0; i < lines.size(); ++i)
    {
        if (i % 2 == 1)
        {
            continue;
        }

        vertices.insert(
            vertices.end(),
            {
                {
                    .pos = lines[i].first + normalUnitVec,
                    .uv = {-1.0f, -1.0f},
                },
                {
                    .pos = lines[i].first - normalUnitVec,
                    .uv = {1.0f, -1.0f},
                },
                {
                    .pos = lines[i].second - normalUnitVec,
                    .uv = {1.0f, 1.0f},
                },
                {
                    .pos = lines[i].second + normalUnitVec,
                    .uv = {-1.0f, 1.0f},
                }
            }
        );

        auto baseIndex = static_cast<uint16_t>(vertices.size() - 4);
        indices.insert(
            indices.end(),
            {
                baseIndex,
                static_cast<uint16_t>(baseIndex + 1),
                static_cast<uint16_t>(baseIndex + 2),
                static_cast<uint16_t>(baseIndex + 2),
                static_cast<uint16_t>(baseIndex + 3),
                baseIndex
            }
        );
    }

    addCapStyle(
        strokeStyle.start_cap_style,
        vertices,
        indices,
        startVec,
        -direction,
        strokeStyle.width,
        dirUnitVec,
        normalUnitVec
    );

    addCapStyle(
        strokeStyle.end_cap_style,
        vertices,
        indices,
        endVec,
        direction,
        strokeStyle.width,
        -dirUnitVec,
        normalUnitVec
    );

    for (int i = 1; i < lines.size(); i++)
    {
        addCapStyle(
          strokeStyle.dash_cap_style,
          vertices,
          indices,
          lines[i - 1].second,
          direction,
          strokeStyle.width,
          -dirUnitVec,
          normalUnitVec
        );
        addCapStyle(
          strokeStyle.dash_cap_style,
          vertices,
          indices,
          lines[i].first,
          -direction,
          strokeStyle.width,
          dirUnitVec,
          normalUnitVec
        );
    }

    return dashOffset;
}

void VulkanGraphicsContextImpl::addCapStyle(
    StrokeStyle::CapStyle capStyle,
    std::vector<VulkanPipeline::Vertex> &vertices,
    std::vector<uint16_t> &indices,
    const glm::vec2 &centerVec,
    const Point &direction,
    float width,
    glm::vec2 dirUnitVec,
    glm::vec2 normalUnitVec
) const
{
    switch (capStyle)
    {
        case StrokeStyle::CapStyle::Butt:
            break;

        case StrokeStyle::CapStyle::Round:
        {
            float baseAngle = std::atan2(direction.y, direction.x);
            float startAngle = baseAngle - std::numbers::pi / 2.0f;
            float endAngle = baseAngle + std::numbers::pi / 2.0f;
            float angleStep = (endAngle - startAngle) / CAP_ROUND_SEGMENTS;
            float radius = width / 2.0f;
            auto pixStart = m_renderer->unNormalize(centerVec);

            for (int i = 0; i <= CAP_ROUND_SEGMENTS; ++i)
            {
                float angle = startAngle + i * angleStep;

                auto pos = pixStart + glm::vec2(std::cos(angle), std::sin(angle)) * radius;

                vertices.push_back({
                    .pos = m_renderer->normalize(pos),
                    .uv = {-1.0f, -1.0f},
                });
            }
            vertices.push_back({
                .pos = centerVec,
                .uv = {0.0f, 0.0f},
            });

            auto baseIndex = static_cast<uint16_t>(vertices.size() - CAP_ROUND_SEGMENTS - 2);
            for (int i = 0; i < CAP_ROUND_SEGMENTS; ++i)
            {
                indices.insert(
                    indices.end(),
                    {
                        static_cast<uint16_t>(baseIndex + i),
                        static_cast<uint16_t>(baseIndex + i + 1),
                        static_cast<uint16_t>(baseIndex + CAP_ROUND_SEGMENTS + 1)
                    }
                );
            }
        }
            break;

        case StrokeStyle::CapStyle::Square:
        {
            vertices.insert(
                vertices.end(),
                {
                    {
                        .pos = centerVec - dirUnitVec + normalUnitVec,
                        .uv = {-1.0f, -1.0f},
                    },
                    {
                        .pos = centerVec - dirUnitVec - normalUnitVec,
                        .uv = {1.0f, -1.0f},
                    },
                    {
                        .pos = centerVec + normalUnitVec,
                        .uv = {1.0f, 1.0f},
                    },
                    {
                        .pos = centerVec - normalUnitVec,
                        .uv = {-1.0f, 1.0f},
                    }
                }
            );
            auto baseIndex = static_cast<uint16_t>(vertices.size() - 4);
            indices.insert(
                indices.end(),
                {
                    baseIndex,
                    static_cast<uint16_t>(baseIndex + 1),
                    static_cast<uint16_t>(baseIndex + 2),
                    static_cast<uint16_t>(baseIndex + 2),
                    static_cast<uint16_t>(baseIndex + 3),
                    baseIndex
                }
            );
        }
            break;

        case StrokeStyle::CapStyle::Triangle:
        {
            vertices.insert(
                vertices.end(),
                {
                    {
                        .pos = centerVec + normalUnitVec,
                        .uv = {-1.0f, -1.0f},
                    },
                    {
                        .pos = centerVec - normalUnitVec,
                        .uv = {1.0f, -1.0f},
                    },
                    {
                        .pos = centerVec - dirUnitVec / 2.0f,
                        .uv = {1.0f, 1.0f},
                    },
                }
            );

            auto baseIndex = static_cast<uint16_t>(vertices.size() - 3);
            indices.insert(
                indices.end(),
                {
                    baseIndex,
                    static_cast<uint16_t>(baseIndex + 1),
                    static_cast<uint16_t>(baseIndex + 2),
                }
            );
        }
            break;
    }
}

float VulkanGraphicsContextImpl::addArc(
    Point center,
    float radiusX,
    float radiusY,
    float startAngle,
    float endAngle,
    const StrokeStyle &strokeStyle,
    std::vector<VulkanPipeline::Vertex> &vertices,
    std::vector<uint16_t> &indices
) const
{
    if (startAngle > endAngle)
    {
        endAngle += 2.0f * std::numbers::pi;
    }

    StrokeStyle style = strokeStyle;

    auto arcPoints = splitArc(center, radiusX, radiusY, startAngle, endAngle);
    for (size_t i = 0; i < arcPoints.size() - 1; ++i)
    {
        float dashOffset = addLine(
            arcPoints[i],
            arcPoints[i + 1],
            style,
            vertices,
            indices
        );
        style.dash_offset = dashOffset;
    }

    return style.dash_offset;
}

std::vector<Point> VulkanGraphicsContextImpl::splitArc(
    Point center,
    float radiusX,
    float radiusY,
    float startAngle,
    float endAngle
) const
{
    constexpr float angleStep = 2.0f * std::numbers::pi / ELLIPSE_SEGMENTS;

    int segments = static_cast<int>(std::ceil((endAngle - startAngle) / angleStep));
    if (segments < 1)
    {
        segments = 1;
    }

    std::vector<Point> points(segments + 1);

    for (int i = 0; i <= segments; ++i)
    {
        float angle = startAngle + i * angleStep;
        if (angle > endAngle)
        {
            angle = endAngle;
        }
        points[i] = {
            center.x + radiusX * std::cos(angle),
            center.y + radiusY * std::sin(angle)
        };
    }

    return points;
}
} // karin