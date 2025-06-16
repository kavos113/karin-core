#include "d2d_device_resources.h"

#include <color/d2d_color.h>

#include <ranges>
#include <stdexcept>

namespace karin
{
void D2DDeviceResources::clear()
{
    for (auto &val: m_solidColorBrushes | std::views::values)
    {
        val.Reset();
    }
}

Microsoft::WRL::ComPtr<ID2D1Brush> D2DDeviceResources::brush(Pattern *pattern)
{
    if (auto solidColorPattern = dynamic_cast<SolidColorPattern*>(pattern))
    {
        return solidColorBrush(*solidColorPattern);
    }

    throw std::runtime_error("Unsupported pattern type for D2D brush");
}

Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> D2DDeviceResources::solidColorBrush(const SolidColorPattern &pattern)
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

Microsoft::WRL::ComPtr<ID2D1StrokeStyle> D2DDeviceResources::strokeStyle(const StrokeStyle &style)
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