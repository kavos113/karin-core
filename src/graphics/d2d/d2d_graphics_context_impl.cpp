#include "d2d_graphics_context_impl.h"

#include "d2d_geometry.h"

#include <algorithm>
#include <stdexcept>
#include <d2d/matrix_converter.h>

#include "karin/graphics/text_blob.h"
#include "windows/dwrite_font_face.h"

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

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(rect.pos.x + rect.size.width / 2, rect.pos.y + rect.size.height / 2);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->FillRectangle(
        D2D1::RectF(-rect.size.width / 2.0f, -rect.size.height / 2.0f, rect.size.width / 2.0f, rect.size.height / 2.0f),
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

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(center.x, center.y);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_ELLIPSE ellipse = {
        D2D1::Point2F(0.0f, 0.0f),
        radiusX, radiusY
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

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(rect.pos.x + rect.size.width / 2, rect.pos.y + rect.size.height / 2);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_ROUNDED_RECT roundedRect = {
        D2D1::RectF(-rect.size.width / 2.0f, -rect.size.height / 2.0f, rect.size.width / 2.0f, rect.size.height / 2.0f),
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

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation((start.x + end.x) / 2, (start.y + end.y) / 2);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->DrawLine(
        D2D1::Point2F(start.x - (start.x + end.x) / 2, start.y - (start.y + end.y) / 2),
        D2D1::Point2F(end.x - (start.x + end.x) / 2, end.y - (start.y + end.y) / 2),
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

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(rect.pos.x + rect.size.width / 2, rect.pos.y + rect.size.height / 2);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    m_deviceContext->DrawRectangle(
        D2D1::RectF(-rect.size.width / 2.0f, -rect.size.height / 2.0f, rect.size.width / 2.0f, rect.size.height / 2.0f),
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

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(center.x, center.y);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_ELLIPSE ellipse = {
        D2D1::Point2F(0.0f, 0.0f),
        radiusX, radiusY
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

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(rect.pos.x + rect.size.width / 2, rect.pos.y + rect.size.height / 2);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * transitionMatrix * oldTransform);

    auto brush = m_deviceResources->brush(pattern);
    if (!brush)
    {
        throw std::runtime_error("Failed to get brush for pattern");
    }

    D2D1_ROUNDED_RECT roundedRect = {
        D2D1::RectF(-rect.size.width / 2.0f, -rect.size.height / 2.0f, rect.size.width / 2.0f, rect.size.height / 2.0f),
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

    D2D1_MATRIX_3X2_F transitionMatrix = D2D1::Matrix3x2F::Translation(destRect.pos.x + destRect.size.width / 2, destRect.pos.y + destRect.size.height / 2);
    m_deviceContext->SetTransform(toD2DMatrix(transform) * transitionMatrix * oldTransform);

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
        D2D1::RectF(-destRect.size.width / 2.0f, -destRect.size.height / 2.0f, destRect.size.width / 2.0f, destRect.size.height / 2.0f),
        opacity,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        srcRectPtr
    );

    m_deviceContext->SetTransform(oldTransform);
}

void D2DGraphicsContextImpl::drawText(const TextBlob& text, Point start, Pattern& pattern, const Transform2D& transform)
{
    auto dwriteFace = dynamic_cast<DwriteFontFace*>(text.fontFace.get());
    if (!dwriteFace)
    {
        throw std::runtime_error("Unsupported font face type");
    }

    std::vector<UINT16> glyphIndices;
    std::vector<DWRITE_GLYPH_OFFSET> glyphOffsets;
    std::vector<float> glyphAdvances;
    glyphIndices.reserve(text.glyphs.size());
    glyphOffsets.reserve(text.glyphs.size());
    glyphAdvances.reserve(text.glyphs.size());

    for (const auto& glyph : text.glyphs)
    {
        glyphIndices.push_back(static_cast<UINT16>(glyph.glyphIndex));
        glyphOffsets.push_back(DWRITE_GLYPH_OFFSET{ glyph.position.pos.x, -glyph.position.pos.y });
        glyphAdvances.push_back(0.0f); // position is calculate by offset
    }

    DWRITE_GLYPH_RUN glyphRun = {
        .fontFace = dwriteFace->face().Get(),
        .fontEmSize = text.fontEmSize,
        .glyphCount = static_cast<UINT32>(text.glyphs.size()),
        .glyphIndices = glyphIndices.data(),
        .glyphAdvances = glyphAdvances.data(),
        .glyphOffsets = glyphOffsets.data(),
        .isSideways = FALSE,
        .bidiLevel = 0
    };

    m_deviceContext->DrawGlyphRun(
        D2D1::Point2F(start.x, start.y),
        &glyphRun,
        m_deviceResources->brush(pattern).Get(),
        DWRITE_MEASURING_MODE_NATURAL
    );
}
} // karin
