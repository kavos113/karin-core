#include <karin/graphics/graphics_context.h>

#include "platform.h"
#include "graphics_context_impl.h"

namespace karin
{
GraphicsContext::GraphicsContext(IRendererImpl* impl)
    : m_rendererImpl(impl)
      , m_impl(createGraphicsContextImpl(impl))
{
}

GraphicsContext::~GraphicsContext() = default;

void GraphicsContext::fillRect(Rectangle rect, Pattern& pattern) const
{
    m_impl->fillRect(rect, pattern);
}

void GraphicsContext::fillEllipse(Point center, float radiusX, float radiusY, Pattern& pattern) const
{
    m_impl->fillEllipse(center, radiusX, radiusY, pattern);
}

void GraphicsContext::fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern& pattern) const
{
    m_impl->fillRoundedRect(rect, radiusX, radiusY, pattern);
}

void GraphicsContext::drawLine(Point start, Point end, Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    m_impl->drawLine(start, end, pattern, strokeStyle);
}

void GraphicsContext::drawRect(Rectangle rect, Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    m_impl->drawRect(rect, pattern, strokeStyle);
}

void GraphicsContext::drawEllipse(
    Point center, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle
) const
{
    m_impl->drawEllipse(center, radiusX, radiusY, pattern, strokeStyle);
}

void GraphicsContext::drawRoundedRect(
    Rectangle rect, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle
) const
{
    m_impl->drawRoundedRect(rect, radiusX, radiusY, pattern, strokeStyle);
}

void GraphicsContext::fillPath(const Path& path, Pattern& pattern) const
{
    m_impl->fillPath(*path.impl(), pattern);
}

void GraphicsContext::drawPath(const Path& path, Pattern& pattern, const StrokeStyle& strokeStyle) const
{
    m_impl->drawPath(*path.impl(), pattern, strokeStyle);
}

void GraphicsContext::drawImage(Image image, Rectangle destRect, Rectangle srcRect, float opacity) const
{
    m_impl->drawImage(image, destRect, srcRect, opacity);
}
} // karin
