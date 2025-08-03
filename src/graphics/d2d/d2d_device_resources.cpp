#include "d2d_device_resources.h"

#include "d2d_geometry.h"
#include "d2d_color.h"

#include <cmath>
#include <numbers>
#include <ranges>
#include <stdexcept>
#include <variant>

namespace karin
{
void D2DDeviceResources::clear()
{
    for (auto& val : m_solidColorBrushes | std::views::values)
    {
        val.Reset();
    }
}

Microsoft::WRL::ComPtr<ID2D1Brush> D2DDeviceResources::brush(Pattern& pattern)
{
    return std::visit(
        [this]<typename T0>(const T0& p) -> Microsoft::WRL::ComPtr<ID2D1Brush>
        {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, SolidColorPattern>)
            {
                return solidColorBrush(p);
            }
            else if constexpr (std::is_same_v<T, LinearGradientPattern>)
            {
                return linearGradientBrush(p);
            }
            else
            {
                throw std::runtime_error("Unsupported pattern type");
            }
        }, pattern
    );
}

Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> D2DDeviceResources::solidColorBrush(const SolidColorPattern& pattern)
{
    if (auto it = m_solidColorBrushes.find(pattern); it != m_solidColorBrushes.end())
    {
        return it->second;
    }

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
    HRESULT hr = m_deviceContext->CreateSolidColorBrush(
        toD2DColor(pattern.color()),
        &brush
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create solid color brush");
    }

    m_solidColorBrushes[pattern] = brush;
    return brush;
}

Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> D2DDeviceResources::linearGradientBrush(
    const LinearGradientPattern& pattern
)
{
    return nullptr;
}

Microsoft::WRL::ComPtr<ID2D1StrokeStyle> D2DDeviceResources::strokeStyle(const StrokeStyle& style)
{
    if (auto it = m_strokeStyles.find(style); it != m_strokeStyles.end())
    {
        return it->second;
    }

    D2D1_STROKE_STYLE_PROPERTIES properties = {
        .startCap = toD2DCapStyle(style.start_cap_style),
        .endCap = toD2DCapStyle(style.end_cap_style),
        .dashCap = toD2DCapStyle(style.dash_cap_style),
        .lineJoin = toD2DJoinStyle(style.join_style),
        .miterLimit = style.miter_limit,
        .dashStyle = style.dash_pattern.empty() ? D2D1_DASH_STYLE_SOLID : D2D1_DASH_STYLE_CUSTOM,
        .dashOffset = style.dash_offset,
    };
    Microsoft::WRL::ComPtr<ID2D1StrokeStyle> strokeStyle;
    const FLOAT* dashes = style.dash_pattern.empty() ? nullptr : style.dash_pattern.data();
    HRESULT hr = m_factory->CreateStrokeStyle(
        properties,
        dashes,
        static_cast<UINT32>(style.dash_pattern.size()),
        &strokeStyle
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D2D stroke style");
    }

    m_strokeStyles[style] = strokeStyle;
    return strokeStyle;
}

Microsoft::WRL::ComPtr<ID2D1PathGeometry> D2DDeviceResources::pathGeometry(const PathImpl& path)
{
    if (auto it = m_pathGeometries.find(path.id()); it != m_pathGeometries.end())
    {
        return it->second;
    }

    Microsoft::WRL::ComPtr<ID2D1PathGeometry> geometry;
    HRESULT hr = m_factory->CreatePathGeometry(&geometry);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D2D path geometry");
    }

    auto commands = path.commands();

    Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
    hr = geometry->Open(&sink);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to open D2D geometry sink");
    }

    sink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
    sink->BeginFigure(toD2DPoint(path.startPoint()), D2D1_FIGURE_BEGIN_FILLED);
    for (const auto& command : commands)
    {
        std::visit(
            [&sink]<typename T0>(const T0& args)
            {
                using T = std::decay_t<T0>;
                if constexpr (std::is_same_v<T, PathImpl::LineArgs>)
                {
                    sink->AddLine(toD2DPoint(args.end));
                }
                else if constexpr (std::is_same_v<T, PathImpl::ArcArgs>)
                {
                    constexpr float twoPi = 2.0f * std::numbers::pi_v<float>;

                    auto end = Point(
                        args.center.x + args.radiusX * std::cos(args.endAngle),
                        args.center.y + args.radiusY * std::sin(-args.endAngle) // bottom is big
                    );
                    float startAngle = std::fmod(args.startAngle, twoPi);
                    float endAngle = std::fmod(args.endAngle, twoPi);

                    if (startAngle < 0.0f)
                    {
                        startAngle += twoPi;
                    }

                    if (endAngle < 0.0f)
                    {
                        endAngle += twoPi;
                    }

                    D2D1_SWEEP_DIRECTION sweepDirection =
                        args.isSmallArc
                            ? (endAngle < startAngle
                                   ? D2D1_SWEEP_DIRECTION_CLOCKWISE
                                   : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE)
                            : (endAngle < startAngle
                                   ? D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE
                                   : D2D1_SWEEP_DIRECTION_CLOCKWISE);
                    sink->AddArc(
                        D2D1::ArcSegment(
                            toD2DPoint(end),
                            D2D1::SizeF(args.radiusX, args.radiusY),
                            0.0f, // rotation angle
                            sweepDirection,
                            args.isSmallArc ? D2D1_ARC_SIZE_SMALL : D2D1_ARC_SIZE_LARGE
                        )
                    );
                }
            }, command
        );
    }
    sink->EndFigure(D2D1_FIGURE_END_CLOSED);
    hr = sink->Close();
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to close D2D geometry sink");
    }

    m_pathGeometries[path.id()] = geometry;
    return geometry;
}

D2D1_CAP_STYLE D2DDeviceResources::toD2DCapStyle(StrokeStyle::CapStyle capStyle)
{
    switch (capStyle)
    {
    case StrokeStyle::CapStyle::Butt:
        return D2D1_CAP_STYLE_FLAT;
    case StrokeStyle::CapStyle::Round:
        return D2D1_CAP_STYLE_ROUND;
    case StrokeStyle::CapStyle::Square:
        return D2D1_CAP_STYLE_SQUARE;
    case StrokeStyle::CapStyle::Triangle:
        return D2D1_CAP_STYLE_TRIANGLE;
    default:
        throw std::invalid_argument("Unknown cap style");
    }
}

D2D1_LINE_JOIN D2DDeviceResources::toD2DJoinStyle(StrokeStyle::JoinStyle joinStyle)
{
    switch (joinStyle)
    {
    case StrokeStyle::JoinStyle::Miter:
        return D2D1_LINE_JOIN_MITER;
    case StrokeStyle::JoinStyle::Round:
        return D2D1_LINE_JOIN_ROUND;
    case StrokeStyle::JoinStyle::Bevel:
        return D2D1_LINE_JOIN_BEVEL;
    default:
        throw std::invalid_argument("Unknown join style");
    }
}
} // karin
