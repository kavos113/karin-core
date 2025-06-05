#include "d2d_graphics_context_impl.h"

#include <geometry/d2d_rectangle.h>

#include <algorithm>
#include <stdexcept>

namespace karin
{
D2DGraphicsContextImpl::D2DGraphicsContextImpl(
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext,
    D2DDeviceResources *deviceResources
)
    : m_deviceContext(std::move(deviceContext)),
      m_deviceResources(deviceResources)
{
}

void D2DGraphicsContextImpl::fillRect(const Rectangle rect, Pattern* pattern, const float strokeWidth)
{
    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->FillRectangle(
        toD2DRect(rect),
        brush.Get()
    );
}
} // karin