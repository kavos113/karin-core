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

    D2D1_MATRIX_3X2_F scaleMatrix = D2D1::Matrix3x2F::Scale(rect.size.width, rect.size.height);
    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(rect.pos.x + rect.size.width / 2, rect.pos.y + rect.size.height / 2);
    m_deviceContext->SetTransform(scaleMatrix * toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->FillRectangle(
        D2D1::RectF(-0.5f, -0.5f, 0.5f, 0.5f),
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

    D2D1_MATRIX_3X2_F scaleMatrix = D2D1::Matrix3x2F::Scale(radiusX, radiusY);
    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(center.x, center.y);
    m_deviceContext->SetTransform(scaleMatrix * toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_ELLIPSE ellipse = {
        D2D1::Point2F(0.0f, 0.0f),
        1.0f,
        1.0f
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

    D2D1_MATRIX_3X2_F scaleMatrix = D2D1::Matrix3x2F::Scale(rect.size.width, rect.size.height);
    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(rect.pos.x + rect.size.width / 2, rect.pos.y + rect.size.height / 2);
    m_deviceContext->SetTransform(scaleMatrix * toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_ROUNDED_RECT roundedRect = {
        D2D1::RectF(-0.5f, -0.5f, 0.5f, 0.5f),
        std::max(0.0f, radiusX / rect.size.width),
        std::max(0.0f, radiusY / rect.size.height)
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

    D2D1_MATRIX_3X2_F scaleMatrix = D2D1::Matrix3x2F::Scale(rect.size.width, rect.size.height);
    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(rect.pos.x + rect.size.width / 2, rect.pos.y + rect.size.height / 2);
    m_deviceContext->SetTransform(scaleMatrix * toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->DrawRectangle(
        D2D1::RectF(-0.5f, -0.5f, 0.5f, 0.5f),
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

    D2D1_MATRIX_3X2_F scaleMatrix = D2D1::Matrix3x2F::Scale(radiusX, radiusY);
    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(center.x, center.y);
    m_deviceContext->SetTransform(scaleMatrix * toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_ELLIPSE ellipse = {
        D2D1::Point2F(0.0f, 0.0f),
        1.0f,
        1.0f
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

    D2D1_MATRIX_3X2_F scaleMatrix = D2D1::Matrix3x2F::Scale(rect.size.width, rect.size.height);
    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(rect.pos.x + rect.size.width / 2, rect.pos.y + rect.size.height / 2);
    m_deviceContext->SetTransform(scaleMatrix * toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_ROUNDED_RECT roundedRect = {
        D2D1::RectF(-0.5f, -0.5f, 0.5f, 0.5f),
        std::max(0.0f, radiusX / rect.size.width),
        std::max(0.0f, radiusY / rect.size.height)
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
