#include "d2d_graphics_context_impl.h"

#include "d2d_geometry.h"

#include <algorithm>
#include <stdexcept>
#include <d2d/matrix_converter.h>

namespace karin
{
D2DGraphicsContextImpl::D2DGraphicsContextImpl(
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext,
    D2DDeviceResources* deviceResources
)
    : m_deviceContext(std::move(deviceContext)),
      m_deviceResources(deviceResources)
{
}

void D2DGraphicsContextImpl::fillRect(const Rectangle rect, Pattern& pattern, const Transform2D& transform)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->FillRectangle(
        toD2DRect(rect),
        brush.Get()
    );

    m_deviceContext->SetTransform(oldTransform);
}

void D2DGraphicsContextImpl::fillEllipse(
    Point center, float radiusX, float radiusY, Pattern& pattern, const Transform2D& transform
)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * oldTransform);

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

    m_deviceContext->SetTransform(oldTransform);
}

void D2DGraphicsContextImpl::fillRoundedRect(
    Rectangle rect, float radiusX, float radiusY, Pattern& pattern, const Transform2D& transform
)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * oldTransform);

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

    m_deviceContext->SetTransform(oldTransform);
}

void D2DGraphicsContextImpl::drawLine(
    Point start, Point end, Pattern& pattern, const StrokeStyle& strokeStyle, const Transform2D& transform
)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->DrawLine(
        toD2DPoint(start),
        toD2DPoint(end),
        brush.Get(),
        strokeStyle.width,
        m_deviceResources->strokeStyle(strokeStyle).Get()
    );

    m_deviceContext->SetTransform(oldTransform);
}

void D2DGraphicsContextImpl::drawRect(
    Rectangle rect, Pattern& pattern, const StrokeStyle& strokeStyle, const Transform2D& transform
)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->DrawRectangle(
        toD2DRect(rect),
        brush.Get(),
        strokeStyle.width,
        m_deviceResources->strokeStyle(strokeStyle).Get()
    );

    m_deviceContext->SetTransform(oldTransform);
}

void D2DGraphicsContextImpl::drawEllipse(
    Point center,
    float radiusX,
    float radiusY,
    Pattern& pattern,
    const StrokeStyle& strokeStyle, const Transform2D& transform
)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * oldTransform);

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
        strokeStyle.width,
        m_deviceResources->strokeStyle(strokeStyle).Get()
    );

    m_deviceContext->SetTransform(oldTransform);
}

void D2DGraphicsContextImpl::drawRoundedRect(
    Rectangle rect,
    float radiusX,
    float radiusY,
    Pattern& pattern,
    const StrokeStyle& strokeStyle, const Transform2D& transform
)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * oldTransform);

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
        strokeStyle.width,
        m_deviceResources->strokeStyle(strokeStyle).Get()
    );

    m_deviceContext->SetTransform(oldTransform);
}

void D2DGraphicsContextImpl::fillPath(const PathImpl& path, Pattern& pattern, const Transform2D& transform)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * oldTransform);

    auto geometry = m_deviceResources->pathGeometry(path);
    if (!geometry)
    {
        throw std::runtime_error("Failed to get path geometry");
    }

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->FillGeometry(
        geometry.Get(),
        brush.Get(),
        nullptr
    );

    m_deviceContext->SetTransform(oldTransform);
}

void D2DGraphicsContextImpl::drawPath(
    const PathImpl& path, Pattern& pattern, const StrokeStyle& strokeStyle, const Transform2D& transform
)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * oldTransform);

    auto geometry = m_deviceResources->pathGeometry(path);
    if (!geometry)
    {
        throw std::runtime_error("Failed to get path geometry");
    }

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->DrawGeometry(
        geometry.Get(),
        brush.Get(),
        strokeStyle.width,
        m_deviceResources->strokeStyle(strokeStyle).Get()
    );

    m_deviceContext->SetTransform(oldTransform);
}

void D2DGraphicsContextImpl::drawImage(
    Image image, Rectangle destRect, Rectangle srcRect, float opacity, const Transform2D& transform
)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * oldTransform);

    auto bitmap = m_deviceResources->bitmap(image);
    if (!bitmap)
    {
        throw std::runtime_error("Failed to get bitmap for image");
    }

    D2D1_RECT_F* srcRectPtr = nullptr;
    if (srcRect != Rectangle())
    {
        static D2D1_RECT_F d2dSrcRect;
        d2dSrcRect = toD2DRect(srcRect);
        srcRectPtr = &d2dSrcRect;
    }

    m_deviceContext->DrawBitmap(
        bitmap.Get(),
        toD2DRect(destRect),
        opacity,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        srcRectPtr
    );

    m_deviceContext->SetTransform(oldTransform);
}

void D2DGraphicsContextImpl::drawText(const TextLayout& text, Point start, Pattern& pattern, const Transform2D& transform)
{
    D2D1_MATRIX_3X2_F oldTransform;
    m_deviceContext->GetTransform(&oldTransform);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * oldTransform);

    auto textLayout = m_deviceResources->textLayout(text);
    if (!textLayout)
    {
        throw std::runtime_error("Failed to get text layout");
    }

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->DrawTextLayout(
        toD2DPoint(start),
        textLayout.Get(),
        brush.Get(),
        D2D1_DRAW_TEXT_OPTIONS_NONE
    );

    m_deviceContext->SetTransform(oldTransform);
}
} // karin
