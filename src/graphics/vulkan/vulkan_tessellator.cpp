#include "vulkan_tessellator.h"

#include "glm_geometry.h"

#include <cmath>
#include <cstdint>
#include <list>
#include <numbers>

namespace
{
using karin::Point;

// det(c-a, b-a)
// det(a, b, c): b->c is counter-clockwise if >0, clockwise if <0  (because: y-axis is down)
float det(const Point& a, const Point& b, const Point& c)
{
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

bool isConvex(const Point& a, const Point& b, const Point& c)
{
    return det(a, b, c) > 0;
}

bool isPointInTriangle(const Point& p, const Point& a, const Point& b, const Point& c)
{
    float p_ab = det(a, b, p);
    float p_bc = det(b, c, p);
    float p_ca = det(c, a, p);

    return (p_ab > 0 && p_bc > 0 && p_ca > 0) || (p_ab < 0 && p_bc < 0 && p_ca < 0);
}
}

namespace karin
{
float VulkanTessellator::addLine(
    Point start,
    Point end,
    const StrokeStyle& strokeStyle,
    std::vector<VulkanPipeline::Vertex>& vertices,
    std::vector<uint16_t>& indices
) const
{
    if (start == end)
    {
        return strokeStyle.dash_offset;
    }

    float dashOffset = 0.0f;

    auto startVec = toGlmVec2(start);
    auto endVec = toGlmVec2(end);

    // 1 unit = 1 width
    auto dirUnitVec = glm::normalize(toGlmVec2(end - start)) * strokeStyle.width;
    // 1 unit = 1/2 width
    auto normalVec = glm::vec2(-dirUnitVec.y, dirUnitVec.x) / 2.0f;

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
                    for (int i = 0; i < dashPatternIndex; ++i)
                    {
                        dashOffset += strokeStyle.dash_pattern[i];
                    }
                }
                else if (dirUnitVec.y != 0.0f)
                {
                    dashOffset = (endVec.y - current.y) / dirUnitVec.y;
                    for (int i = 0; i < dashPatternIndex; ++i)
                    {
                        dashOffset += strokeStyle.dash_pattern[i];
                    }
                }
                break;
            }

            line.second = current + dirUnitVec * strokeStyle.dash_pattern[dashPatternIndex];
            lines.push_back(line);

            current = line.second;
            dashPatternIndex = (dashPatternIndex + 1) % strokeStyle.dash_pattern.size();
        }
    }

    int startIndex = 0;
    int endIndex = static_cast<int>(lines.size()) - 1;
    if (endIndex % 2 == 1)
    {
        endIndex -= 1; // Ensure the last line is not a blank line
    }

    for (int i = 0; i < lines.size(); ++i)
    {
        // blank
        if (i % 2 == 1)
        {
            continue;
        }

        // Skip lines that are before the start point
        if ((lines[i].second.x - startVec.x) / dirUnitVec.x < 0.0f
            || (lines[i].second.y - startVec.y) / dirUnitVec.y < 0.0f)
        {
            startIndex = i + 2;
            continue;
        }

        // Skip lines that are after the end point
        if ((lines[i].first.x - endVec.x) / dirUnitVec.x > 0.0f
            || (lines[i].first.y - endVec.y) / dirUnitVec.y > 0.0f)
        {
            endIndex = i - 2;
            continue;
        }

        // Adjust the first point to the start point
        if ((lines[i].first.x - startVec.x) / dirUnitVec.x < 0.0f
            || (lines[i].first.y - startVec.y) / dirUnitVec.y < 0.0f)
        {
            lines[i].first = startVec;
        }

        // Skip zero length lines
        if (lines[i].first == lines[i].second)
        {
            continue;
        }

        vertices.insert(
            vertices.end(),
            {
                {
                    .pos = lines[i].first + normalVec,
                    .uv = {-1.0f, -1.0f},
                },
                {
                    .pos = lines[i].first - normalVec,
                    .uv = {1.0f, -1.0f},
                },
                {
                    .pos = lines[i].second - normalVec,
                    .uv = {1.0f, 1.0f},
                },
                {
                    .pos = lines[i].second + normalVec,
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

    if (startIndex > endIndex)
    {
        // No lines to draw
        return dashOffset;
    }

    addCapStyle(
        strokeStyle.start_cap_style,
        vertices,
        indices,
        lines[startIndex].first,
        dirUnitVec,
        normalVec,
        strokeStyle.width
    );

    addCapStyle(
        strokeStyle.end_cap_style,
        vertices,
        indices,
        lines[endIndex].second,
        -dirUnitVec,
        normalVec,
        strokeStyle.width
    );

    for (int i = startIndex + 2; i <= endIndex; i += 2)
    {
        addCapStyle(
            strokeStyle.dash_cap_style,
            vertices,
            indices,
            lines[i - 2].second,
            -dirUnitVec,
            normalVec,
            strokeStyle.width
        );
        addCapStyle(
            strokeStyle.dash_cap_style,
            vertices,
            indices,
            lines[i].first,
            dirUnitVec,
            normalVec,
            strokeStyle.width
        );
    }

    return dashOffset;
}

float VulkanTessellator::addArc(
    Point center,
    float radiusX,
    float radiusY,
    float startAngle,
    float endAngle,
    bool isClockwise,
    const StrokeStyle& strokeStyle,
    std::vector<VulkanPipeline::Vertex>& vertices,
    std::vector<uint16_t>& indices
) const
{
    StrokeStyle style = strokeStyle;

    auto arcPoints = splitArc(center, radiusX, radiusY, startAngle, endAngle, isClockwise);
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

void VulkanTessellator::addCapStyle(
    const StrokeStyle::CapStyle capStyle,
    std::vector<VulkanPipeline::Vertex>& vertices,
    std::vector<uint16_t>& indices,
    const glm::vec2& centerVec,
    const glm::vec2& dirUnitVec,
    const glm::vec2& normalVec,
    const float width
) const
{
    switch (capStyle)
    {
    case StrokeStyle::CapStyle::Butt:
        break;

    case StrokeStyle::CapStyle::Round:
    {
        float baseAngle = std::atan2(-dirUnitVec.y, -dirUnitVec.x);
        float startAngle = baseAngle - std::numbers::pi / 2.0f;
        float endAngle = baseAngle + std::numbers::pi / 2.0f;
        float angleStep = (endAngle - startAngle) / CAP_ROUND_SEGMENTS;
        float radius = width / 2.0f;

        for (int i = 0; i <= CAP_ROUND_SEGMENTS; ++i)
        {
            float angle = startAngle + i * angleStep;
            auto pos = centerVec + glm::vec2(std::cos(angle), std::sin(angle)) * radius;

            vertices.push_back(
                {
                    .pos = pos,
                    .uv = {-1.0f, -1.0f},
                }
            );
        }
        vertices.push_back(
            {
                .pos = centerVec,
                .uv = {0.0f, 0.0f},
            }
        );

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
                    .pos = centerVec - dirUnitVec + normalVec,
                    .uv = {-1.0f, -1.0f},
                },
                {
                    .pos = centerVec - dirUnitVec - normalVec,
                    .uv = {1.0f, -1.0f},
                },
                {
                    .pos = centerVec + normalVec,
                    .uv = {1.0f, 1.0f},
                },
                {
                    .pos = centerVec - normalVec,
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
                    .pos = centerVec + normalVec,
                    .uv = {-1.0f, -1.0f},
                },
                {
                    .pos = centerVec - normalVec,
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

std::vector<Point> VulkanTessellator::splitArc(
    Point center,
    float radiusX,
    float radiusY,
    float startAngle,
    float endAngle,
    bool isClockwise
)
{
    if (isClockwise)
    {
        if (startAngle < endAngle)
        {
            startAngle += 2.0f * std::numbers::pi;
        }
    }
    else
    {
        if (startAngle > endAngle)
        {
            endAngle += 2.0f * std::numbers::pi;
        }
    }

    constexpr float angleStep = 2.0f * std::numbers::pi / ELLIPSE_SEGMENTS;

    int segments = std::floor(std::abs(endAngle - startAngle) / angleStep);
    if (segments < 1)
    {
        segments = 1;
    }

    std::vector<Point> points(segments + 2);

    for (int i = 0; i <= segments + 1; ++i)
    {
        float angle = startAngle + angleStep * (isClockwise ? -i : i);
        if ((isClockwise && angle < endAngle) || (!isClockwise && angle > endAngle))
        {
            angle = endAngle;
        }
        points[i] = {
            center.x + radiusX * std::cos(angle),
            center.y + radiusY * std::sin(-angle)
        };
    }

    return points;
}

std::vector<uint16_t> VulkanTessellator::triangulate(const std::vector<Point>& polygon)
{
    std::vector<uint16_t> triangleIndices;

    if (polygon.size() < 3)
    {
        return triangleIndices;
    }

    if (polygon.size() == 3)
    {
        triangleIndices = {0, 1, 2};
        return triangleIndices;
    }

    std::list<uint16_t> indices;
    for (int i = 0; i < polygon.size(); i++)
    {
        indices.push_back(i);
    }

    // ensure counter clockwise order
    float signedArea = 0;
    for (size_t i = 0; i < polygon.size(); i++)
    {
        size_t next = (i + 1) % polygon.size();
        signedArea += det({0, 0}, polygon[i], polygon[next]);
    }
    if (signedArea < 0)
    {
        std::reverse(indices.begin(), indices.end());
    }

    while (indices.size() > 3)
    {
        for (int i = 0; i < indices.size(); i++)
        {
            int prev = (i + indices.size() - 1) % indices.size();
            int curr = i;
            int next = (i + 1) % indices.size();

            uint16_t i_prev = *std::next(indices.begin(), prev);
            uint16_t i_curr = *std::next(indices.begin(), curr);
            uint16_t i_next = *std::next(indices.begin(), next);

            const Point& p_p = polygon[i_prev];
            const Point& p_c = polygon[i_curr];
            const Point& p_n = polygon[i_next];

            if (!isConvex(p_p, p_c, p_n))
            {
                continue;
            }

            bool isEar = true;
            for (int index : indices)
            {
                if (index == i_prev || index == i_curr || index == i_next)
                {
                    continue;
                }

                if (isPointInTriangle(polygon[index], p_p, p_c, p_n))
                {
                    isEar = false;
                    break;
                }
            }
            if (!isEar)
            {
                continue;
            }

            triangleIndices.push_back(i_prev);
            triangleIndices.push_back(i_curr);
            triangleIndices.push_back(i_next);
            indices.erase(std::next(indices.begin(), curr));
            break;
        }
    }

    if (indices.size() == 3)
    {
        triangleIndices.push_back(*indices.begin());
        triangleIndices.push_back(*std::next(indices.begin(), 1));
        triangleIndices.push_back(*std::next(indices.begin(), 2));
    }

    return triangleIndices;
}
} // karin
