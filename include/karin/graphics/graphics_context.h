#ifndef KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
#define KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
#include <memory>

#include <karin/common/geometry/rectangle.h>
#include <karin/common/geometry/point.h>
#include <karin/common/color/pattern.h>

#include "path.h"
#include "stroke_style.h"

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
    void fillRect(Rectangle rect, Pattern& pattern) const;
    void fillEllipse(Point center, float radiusX, float radiusY, Pattern& pattern) const;
    void fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern& pattern) const;
    void fillPath(const Path& path, Pattern& pattern) const;

    void drawLine(Point start, Point end, Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle()) const;
    void drawRect(Rectangle rect, Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle()) const;
    void drawEllipse(
        Point center, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle()
    ) const;
    void drawRoundedRect(
        Rectangle rect, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle()
    ) const;
    void drawPath(const Path& path, Pattern& pattern, const StrokeStyle& strokeStyle = StrokeStyle()) const;

private:
    IRendererImpl* m_rendererImpl;

    std::unique_ptr<IGraphicsContextImpl> m_impl;
};
} // karin

#endif //KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
