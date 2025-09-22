#ifndef KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
#define KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H

#include <memory>

#include <karin/common/geometry/rectangle.h>
#include <karin/common/geometry/point.h>
#include <karin/common/geometry/transform2d.h>

#include "path.h"
#include "stroke_style.h"
#include "pattern.h"
#include "image.h"
#include "text_layout.h"

namespace karin
{
class IRendererImpl;
class IGraphicsContextImpl;

/**
 * GraphicsContext provides basic drawing operations(e.g., drawRect).
 *
 * This class is only created by Renderer.
 */
class GraphicsContext
{
private:
    explicit GraphicsContext(IRendererImpl* impl);
    ~GraphicsContext();

    friend class Renderer;

public:
    void fillRect(Rectangle rect, Pattern& pattern, const Transform2D& transform = Transform2D()) const;
    void fillEllipse(
        Point center, float radiusX, float radiusY, Pattern& pattern, Transform2D transform = Transform2D()
    ) const;
    void fillRoundedRect(
        Rectangle rect, float radiusX, float radiusY, Pattern& pattern, Transform2D transform = Transform2D()
    ) const;
    void fillPath(const Path& path, Pattern& pattern, Transform2D transform = Transform2D()) const;

    void drawLine(
        Point start, Point end, Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle(),
        Transform2D transform = Transform2D()
    ) const;
    void drawRect(
        Rectangle rect, Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle(),
        Transform2D transform = Transform2D()
    ) const;
    void drawEllipse(
        Point center, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle(),
        Transform2D transform = Transform2D()
    ) const;
    void drawRoundedRect(
        Rectangle rect, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle(),
        Transform2D transform = Transform2D()
    ) const;
    void drawPath(
        const Path& path, Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle(),
        Transform2D transform = Transform2D()
    ) const;

    void drawImage(
        Image image, Rectangle destRect, Rectangle srcRect = Rectangle(), float opacity = 1.0f,
        Transform2D transform = Transform2D()
    ) const;
    void drawText(const TextLayout& text, Point start, Pattern& pattern, Transform2D transform = Transform2D()) const;

private:
    IRendererImpl* m_rendererImpl;

    std::unique_ptr<IGraphicsContextImpl> m_impl;
};
} // karin

#endif //KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
