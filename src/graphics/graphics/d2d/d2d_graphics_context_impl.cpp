#include "d2d_graphics_context_impl.h"

#include <geometry/d2d_geometry.h>

#include <algorithm>
#include <stdexcept>

#include "karin/graphics/graphics/stroke_style.h"

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

void D2DGraphicsContextImpl::fillRect(const Rectangle rect, Pattern* pattern)
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

void D2DGraphicsContextImpl::fillEllipse(Point center, float radiusX, float radiusY, Pattern *pattern)
{
    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_ELLIPSE ellipse = {
        toD2DPoint(center),
        radiusX,
        radiusY
    };
    m_deviceContext->FillEllipse(
        ellipse,
        brush.Get()
    );
}

void D2DGraphicsContextImpl::fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern *pattern)
{
    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_ROUNDED_RECT roundedRect = {
        toD2DRect(rect),
        std::max(0.0f, radiusX),
        std::max(0.0f, radiusY)
    };
    m_deviceContext->FillRoundedRectangle(
        roundedRect,
        brush.Get()
    );
}

void D2DGraphicsContextImpl::drawLine(Point start, Point end, Pattern *pattern, StrokeStyle strokeStyle)
{
    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->DrawLine(
        toD2DPoint(start),
        toD2DPoint(end),
        brush.Get(),
        strokeStyle
    );
}

void D2DGraphicsContextImpl::drawRect(Rectangle rect, Pattern *pattern, StrokeStyle strokeStyle)
{
    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->DrawRectangle(
        toD2DRect(rect),
        brush.Get(),
        strokeStyle
    );
}

void D2DGraphicsContextImpl::drawEllipse(
    Point center,
    float radiusX,
    float radiusY,
    Pattern *pattern,
    StrokeStyle strokeStyle
)
{
    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_ELLIPSE ellipse = {
        toD2DPoint(center),
        radiusX,
        radiusY
    };
    m_deviceContext->DrawEllipse(
        ellipse,
        brush.Get(),
        strokeStyle
    );
}

void D2DGraphicsContextImpl::drawRoundedRect(
    Rectangle rect,
    float radiusX,
    float radiusY,
    Pattern *pattern,
    StrokeStyle strokeStyle
)
{
    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_ROUNDED_RECT roundedRect = {
        toD2DRect(rect),
        std::max(0.0f, radiusX),
        std::max(0.0f, radiusY)
    };
    m_deviceContext->DrawRoundedRectangle(
        roundedRect,
        brush.Get(),
        strokeStyle
    );
}
} // karin