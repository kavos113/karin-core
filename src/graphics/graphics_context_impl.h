#ifndef SRC_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_IMPL_H

#include "path_impl.h"

#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/stroke_style.h>
#include <karin/graphics/image.h>

namespace karin
{
class IGraphicsContextImpl
{
public:
    virtual ~IGraphicsContextImpl() = default;

    virtual void fillRect(Rectangle rect, Pattern& pattern) = 0;
    virtual void fillEllipse(Point center, float radiusX, float radiusY, Pattern& pattern) = 0;
    virtual void fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern& pattern) = 0;
    virtual void fillPath(const PathImpl& path, Pattern& pattern) = 0;

    virtual void drawLine(Point start, Point end, Pattern& pattern, const StrokeStyle& strokeStyle) = 0;
    virtual void drawRect(Rectangle rect, Pattern& pattern, const StrokeStyle& strokeStyle) = 0;
    virtual void drawEllipse(
        Point center, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle
    ) = 0;
    virtual void drawRoundedRect(
        Rectangle rect, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle
    ) = 0;
    virtual void drawPath(const PathImpl& path, Pattern& pattern, const StrokeStyle& strokeStyle) = 0;

    virtual void drawImage(Image image, Rectangle destRect, Rectangle srcRect = Rectangle(), float opacity = 1.0f) = 0;
};
} // karin

#endif //SRC_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_IMPL_H
