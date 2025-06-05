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

Microsoft::WRL::ComPtr<ID2D1Brush> D2DDeviceResources::brush(const Pattern &pattern)
{
    if (auto solidColorPattern = dynamic_cast<const SolidColorPattern*>(&pattern))
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
    if (SUCCEEDED(m_deviceContext->CreateSolidColorBrush(
            toD2DColor(pattern.color()),
            &brush)))
    {
        m_solidColorBrushes[pattern] = brush;
        return brush;
    }

    throw std::runtime_error("Failed to create solid color brush");
}
} // karin