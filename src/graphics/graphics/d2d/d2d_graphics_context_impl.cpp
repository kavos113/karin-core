#include "d2d_graphics_context_impl.h"

#include <algorithm>

namespace karin
{
D2DGraphicsContextImpl::D2DGraphicsContextImpl(Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext)
    : m_deviceContext(std::move(deviceContext))
{
}

void D2DGraphicsContextImpl::drawRect(const Rectangle rect, const float strokeWidth)
{
    m_deviceContext->DrawRectangle(
        D2D1::RectF(
            rect.pos.x,
            rect.pos.y,
            rect.pos.x + rect.size.width,
            rect.pos.y + rect.size.height
        ),
        nullptr, // TODO: use a brush for color
        strokeWidth
    );
}
} // karin