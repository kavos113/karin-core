#include "vulkan_graphics_context_impl.h"

#include "glm_geometry.h"
#include "vulkan_renderer_impl.h"

#include <karin/common/color/color.h>
#include <karin/common/geometry/point.h>
#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/stroke_style.h>

#include <cmath>
#include <iostream>
#include <numbers>
#include <stdexcept>
#include <variant>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace karin
{
VulkanGraphicsContextImpl::VulkanGraphicsContextImpl(VulkanRendererImpl* renderer)
    : m_renderer(renderer)
{
    m_tessellator = std::make_unique<VulkanTessellator>(m_renderer);
}

void VulkanGraphicsContextImpl::fillRect(Rectangle rect, Pattern& pattern)
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
            .pos = {
                normalizedRect.pos.x + normalizedRect.size.width, normalizedRect.pos.y + normalizedRect.size.height
            },
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

    m_renderer->addCommand(vertices, indices, createPushConstantData(pattern), pattern, true);
}

void VulkanGraphicsContextImpl::fillEllipse(Point center, float radiusX, float radiusY, Pattern& pattern)
{
    Rectangle rect = m_renderer->normalize(
        Rectangle(
            center.x - radiusX,
            center.y - radiusY,
            radiusX * 2.0f,
            radiusY * 2.0f
        )
    );

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

    auto fragData = createPushConstantData(pattern);
    fragData.shapeType = static_cast<uint32_t>(ShapeType::Ellipse);

    m_renderer->addCommand(vertices, indices, fragData, pattern, true);
}

void VulkanGraphicsContextImpl::fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern& pattern)
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
            .pos = {
                normalizedRect.pos.x + normalizedRect.size.width, normalizedRect.pos.y + normalizedRect.size.height
            },
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

    auto fragData = createPushConstantData(pattern);
    fragData.shapeType = static_cast<uint32_t>(ShapeType::RoundedRectangle);
    fragData.shapeParams = glm::vec2(radiusX / rect.size.width * 2.0f, radiusY / rect.size.height * 2.0f);

    m_renderer->addCommand(vertices, indices, fragData, pattern, true);
}

void VulkanGraphicsContextImpl::drawLine(Point start, Point end, Pattern& pattern, const StrokeStyle& strokeStyle)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    m_tessellator->addLine(start, end, strokeStyle, vertices, indices);

    m_renderer->addCommand(vertices, indices, createPushConstantData(pattern), pattern, true);
}

void VulkanGraphicsContextImpl::drawRect(Rectangle rect, Pattern& pattern, const StrokeStyle& strokeStyle)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    StrokeStyle style = strokeStyle;
    style.start_cap_style = style.dash_cap_style;
    style.end_cap_style = style.dash_cap_style;
    float dashOffset = m_tessellator->addLine(
        Point(rect.pos.x, rect.pos.y),
        Point(rect.pos.x + rect.size.width, rect.pos.y),
        style,
        vertices,
        indices
    );
    style.dash_offset = dashOffset;
    dashOffset = m_tessellator->addLine(
        Point(rect.pos.x + rect.size.width, rect.pos.y),
        Point(rect.pos.x + rect.size.width, rect.pos.y + rect.size.height),
        style,
        vertices,
        indices
    );
    style.dash_offset = dashOffset;
    dashOffset = m_tessellator->addLine(
        Point(rect.pos.x + rect.size.width, rect.pos.y + rect.size.height),
        Point(rect.pos.x, rect.pos.y + rect.size.height),
        style,
        vertices,
        indices
    );
    style.dash_offset = dashOffset;
    m_tessellator->addLine(
        Point(rect.pos.x, rect.pos.y + rect.size.height),
        Point(rect.pos.x, rect.pos.y),
        style,
        vertices,
        indices
    );

    m_renderer->addCommand(vertices, indices, createPushConstantData(pattern), pattern, true);
}

void VulkanGraphicsContextImpl::drawEllipse(
    Point center, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle
)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    StrokeStyle style = strokeStyle;
    style.start_cap_style = style.dash_cap_style;
    style.end_cap_style = style.dash_cap_style;

    m_tessellator->addArc(
        center,
        radiusX,
        radiusY,
        0.0f,
        2.0f * std::numbers::pi,
        false,
        style,
        vertices,
        indices
    );

    m_renderer->addCommand(vertices, indices, createPushConstantData(pattern), pattern, true);
}

void VulkanGraphicsContextImpl::drawRoundedRect(
    Rectangle rect,
    float radiusX,
    float radiusY,
    Pattern& pattern,
    const StrokeStyle& strokeStyle
)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    StrokeStyle style = strokeStyle;
    style.start_cap_style = style.dash_cap_style;
    style.end_cap_style = style.dash_cap_style;

    float offset = m_tessellator->addArc(
        Point(rect.pos.x + radiusX, rect.pos.y + radiusY),
        radiusX,
        radiusY,
        std::numbers::pi,
        0.5f * std::numbers::pi,
        true,
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = m_tessellator->addLine(
        Point(rect.pos.x + radiusX, rect.pos.y),
        Point(rect.pos.x + rect.size.width - radiusX, rect.pos.y),
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = m_tessellator->addArc(
        Point(rect.pos.x + rect.size.width - radiusX, rect.pos.y + radiusY),
        radiusX,
        radiusY,
        0.5f * std::numbers::pi,
        0.0f,
        true,
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = m_tessellator->addLine(
        Point(rect.pos.x + rect.size.width, rect.pos.y + radiusY),
        Point(rect.pos.x + rect.size.width, rect.pos.y + rect.size.height - radiusY),
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = m_tessellator->addArc(
        Point(rect.pos.x + rect.size.width - radiusX, rect.pos.y + rect.size.height - radiusY),
        radiusX,
        radiusY,
        0.0f,
        1.5f * std::numbers::pi,
        true,
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = m_tessellator->addLine(
        Point(rect.pos.x + rect.size.width - radiusX, rect.pos.y + rect.size.height),
        Point(rect.pos.x + radiusX, rect.pos.y + rect.size.height),
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    offset = m_tessellator->addArc(
        Point(rect.pos.x + radiusX, rect.pos.y + rect.size.height - radiusY),
        radiusX,
        radiusY,
        1.5f * std::numbers::pi,
        std::numbers::pi,
        true,
        style,
        vertices,
        indices
    );
    style.dash_offset = offset;
    m_tessellator->addLine(
        Point(rect.pos.x, rect.pos.y + rect.size.height - radiusY),
        Point(rect.pos.x, rect.pos.y + radiusY),
        style,
        vertices,
        indices
    );

    m_renderer->addCommand(vertices, indices, createPushConstantData(pattern), pattern, true);
}

void VulkanGraphicsContextImpl::fillPath(const PathImpl& path, Pattern& pattern)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    std::vector<Point> polygonPoints;

    auto commands = path.commands();

    for (const auto& command : commands)
    {
        std::visit(
            [this, &polygonPoints]<typename T0>(const T0& args)
            {
                using T = std::decay_t<T0>;
                if constexpr (std::is_same_v<T, PathImpl::LineArgs>)
                {
                    if (polygonPoints.empty() || polygonPoints.back() != args.end)
                    {
                        polygonPoints.push_back(args.end);
                    }
                }
                else if constexpr (std::is_same_v<T, PathImpl::ArcArgs>)
                {
                    bool isClockwise = args.isSmallArc
                                           ? (args.endAngle < args.startAngle)
                                           : (args.endAngle > args.startAngle);

                    auto arcPoints = VulkanTessellator::splitArc(
                        args.center,
                        args.radiusX,
                        args.radiusY,
                        args.startAngle,
                        args.endAngle,
                        isClockwise
                    );

                    if (arcPoints.size() < 2)
                    {
                        return; // No points to add
                    }

                    for (int i = 1; i < arcPoints.size(); ++i)
                    {
                        if (polygonPoints.empty() || polygonPoints.back() != arcPoints[i])
                        {
                            polygonPoints.push_back(arcPoints[i]);
                        }
                    }
                }
            },
            command
        );
    }

    for (auto point : polygonPoints)
    {
        Point normalizedPoint = m_renderer->normalize(point);
        vertices.push_back(
            {
                .pos = {normalizedPoint.x, normalizedPoint.y},
                .uv = {-1.0f, -1.0f} // UV coordinates are not used for fill
            }
        );
    }

    auto triangles = VulkanTessellator::triangulate(polygonPoints);
    for (size_t i = 0; i < triangles.size(); i += 3)
    {
        indices.push_back(triangles[i]);
        indices.push_back(triangles[i + 1]);
        indices.push_back(triangles[i + 2]);
    }

    m_renderer->addCommand(vertices, indices, createPushConstantData(pattern), pattern, true);
}

void VulkanGraphicsContextImpl::drawPath(const PathImpl& path, Pattern& pattern, const StrokeStyle& strokeStyle)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    StrokeStyle style = strokeStyle;
    style.start_cap_style = style.dash_cap_style;
    style.end_cap_style = style.dash_cap_style;

    auto commands = path.commands();
    Point currentPoint = path.startPoint();

    for (const auto& command : commands)
    {
        std::visit(
            [&style, this, &vertices, &indices, &currentPoint]<typename T0>(const T0& args)
            {
                using T = std::decay_t<T0>;
                if constexpr (std::is_same_v<T, PathImpl::LineArgs>)
                {
                    float offset = m_tessellator->addLine(
                        currentPoint,
                        args.end,
                        style,
                        vertices,
                        indices
                    );

                    currentPoint = args.end;
                    style.dash_offset = offset;
                }
                else if constexpr (std::is_same_v<T, PathImpl::ArcArgs>)
                {
                    bool isClockwise = args.isSmallArc
                                           ? (args.endAngle < args.startAngle)
                                           : (args.endAngle > args.startAngle);

                    float offset = m_tessellator->addArc(
                        args.center,
                        args.radiusX,
                        args.radiusY,
                        args.startAngle,
                        args.endAngle,
                        isClockwise,
                        style,
                        vertices,
                        indices
                    );
                    style.dash_offset = offset;

                    currentPoint = Point(
                        args.center.x + args.radiusX * std::cos(args.endAngle),
                        args.center.y + args.radiusY * std::sin(-args.endAngle) // bottom is big
                    );
                }
            },
            command
        );
    }

    m_renderer->addCommand(vertices, indices, createPushConstantData(pattern), pattern, true);
}

void VulkanGraphicsContextImpl::drawImage(Image image, Rectangle destRect, Rectangle srcRect, float opacity)
{
    Rectangle normalizedRect = m_renderer->normalize(destRect);

    Rectangle normalizedSrcRect{
        srcRect.pos.x / image.width(),
        srcRect.pos.y / image.height(),
        srcRect.size.width / image.width(),
        srcRect.size.height / image.height()
    };

    if (srcRect == Rectangle())
    {
        normalizedSrcRect = Rectangle(0.0f, 0.0f, 1.0f, 1.0f);
    }

    std::vector<VulkanPipeline::Vertex> vertices = {
        {
            .pos = {normalizedRect.pos.x, normalizedRect.pos.y},
            .uv = {normalizedSrcRect.pos.x, normalizedSrcRect.pos.y},
        },
        {
            .pos = {normalizedRect.pos.x + normalizedRect.size.width, normalizedRect.pos.y},
            .uv = {normalizedSrcRect.pos.x + normalizedSrcRect.size.width, normalizedSrcRect.pos.y},
        },
        {
            .pos = {
                normalizedRect.pos.x + normalizedRect.size.width, normalizedRect.pos.y + normalizedRect.size.height
            },
            .uv = {
                normalizedSrcRect.pos.x + normalizedSrcRect.size.width,
                normalizedSrcRect.pos.y + normalizedSrcRect.size.height
            },
        },
        {
            .pos = {normalizedRect.pos.x, normalizedRect.pos.y + normalizedRect.size.height},
            .uv = {normalizedSrcRect.pos.x, normalizedSrcRect.pos.y + normalizedSrcRect.size.height},
        }
    };

    std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    ImagePattern imagePattern{
        .image = std::move(image),
        .offset = Point(0.0f, 0.0f),
        .scaleX = normalizedSrcRect.size.width,
        .scaleY = normalizedSrcRect.size.height
    };
    PushConstants pushConstants = createPushConstantData(imagePattern);
    pushConstants.global.x = 0.0f;

    m_renderer->addCommand(vertices, indices, pushConstants, imagePattern, true);
}

void VulkanGraphicsContextImpl::drawText(const TextLayout& text, Point start, Pattern& pattern)
{
    std::vector<VulkanPipeline::Vertex> vertices;
    std::vector<uint16_t> indices;

    auto glyphInfos = m_renderer->deviceResources()->textLayout(text);

    for (const auto& glyphInfo : glyphInfos)
    {
        Rectangle pos = m_renderer->normalize(
            Rectangle(
                start.x + glyphInfo.position.pos.x,
                start.y + glyphInfo.position.pos.y,
                glyphInfo.position.size.width,
                glyphInfo.position.size.height
            )
        );

        size_t baseIndex = vertices.size();

        vertices.push_back(
            {
                .pos = {pos.pos.x, pos.pos.y},
                .uv = {glyphInfo.uv.pos.x, glyphInfo.uv.pos.y}
            }
        );
        vertices.push_back(
            {
                .pos = {pos.pos.x + pos.size.width, pos.pos.y},
                .uv = {glyphInfo.uv.pos.x + glyphInfo.uv.size.width, glyphInfo.uv.pos.y}
            }
        );
        vertices.push_back(
            {
                .pos = {pos.pos.x + pos.size.width, pos.pos.y + pos.size.height},
                .uv = {
                    glyphInfo.uv.pos.x + glyphInfo.uv.size.width,
                    glyphInfo.uv.pos.y + glyphInfo.uv.size.height
                }
            }
        );
        vertices.push_back(
            {
                .pos = {pos.pos.x, pos.pos.y + pos.size.height},
                .uv = {glyphInfo.uv.pos.x, glyphInfo.uv.pos.y + glyphInfo.uv.size.height}
            }
        );

        indices.push_back(static_cast<uint16_t>(baseIndex));
        indices.push_back(static_cast<uint16_t>(baseIndex + 1));
        indices.push_back(static_cast<uint16_t>(baseIndex + 2));
        indices.push_back(static_cast<uint16_t>(baseIndex + 2));
        indices.push_back(static_cast<uint16_t>(baseIndex + 3));
        indices.push_back(static_cast<uint16_t>(baseIndex));
    }

    m_renderer->addCommand(vertices, indices, createPushConstantData(pattern), pattern, true);
}

PushConstants VulkanGraphicsContextImpl::createPushConstantData(const Pattern& pattern) const
{
    return std::visit(
        [this]<typename T0>(const T0& p) -> PushConstants
        {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, SolidColorPattern>)
            {
                Color color = p.color();
                return PushConstants{
                    .color = {color.r, color.g, color.b, color.a},
                    .patternType = static_cast<uint32_t>(PatternType::SolidColor)
                };
            }
            else if constexpr (std::is_same_v<T, LinearGradientPattern>)
            {
                Point start = m_renderer->normalize(p.start);
                Point end = m_renderer->normalize(p.end);
                return PushConstants{
                    .color = {start.x, start.y, end.x, end.y},
                    .patternType = static_cast<uint32_t>(PatternType::LinearGradient),
                };
            }
            else if constexpr (std::is_same_v<T, RadialGradientPattern>)
            {
                Point center = m_renderer->normalize(p.center);
                glm::vec2 offset = m_renderer->normalizeVec(glm::vec2(p.offset.x, p.offset.y));
                glm::vec2 radius = m_renderer->normalizeVec(glm::vec2(p.radiusX, p.radiusY));
                return PushConstants{
                    .color = {center.x, center.y, offset.x, offset.y},
                    .patternType = static_cast<uint32_t>(PatternType::RadialGradient),
                    .global = {radius.x, radius.y},
                };
            }
            else if constexpr (std::is_same_v<T, ImagePattern>)
            {
                glm::vec2 offset = m_renderer->normalizeVec(glm::vec2(p.offset.x, p.offset.y));
                float scaleX = p.scaleX;
                float scaleY = p.scaleY;
                return PushConstants{
                    .color = {offset.x, offset.y, scaleX, scaleY},
                    .patternType = static_cast<uint32_t>(PatternType::Image),
                    .global = {1.0f, 0.0f}
                };
            }
            else
            {
                throw std::runtime_error("Unsupported pattern type");
            }
        }, pattern
    );
}
} // karin
