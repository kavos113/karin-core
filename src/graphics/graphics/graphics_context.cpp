#include <karin/graphics/graphics/graphics_context.h>

#include "platform.h"

namespace karin
{
GraphicsContext::GraphicsContext(IRendererImpl *impl)
    : m_rendererImpl(impl)
    , m_impl(createGraphicsContextImpl(impl))
{
}

GraphicsContext::~GraphicsContext() = default;

void GraphicsContext::fillRect(Rectangle rect, Pattern *pattern)
{
    m_impl->fillRect(rect, pattern);
}

void GraphicsContext::fillEllipse(Point center, float radiusX, float radiusY, Pattern *pattern)
{
    m_impl->fillEllipse(center, radiusX, radiusY, pattern);
}

void GraphicsContext::fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern *pattern)
{
    m_impl->fillRoundedRect(rect, radiusX, radiusY, pattern);
}

void GraphicsContext::drawLine(Point start, Point end, Pattern *pattern, StrokeStyle strokeStyle)
{
    m_impl->drawLine(start, end, pattern, strokeStyle);
}

void GraphicsContext::drawRect(Rectangle rect, Pattern *pattern, StrokeStyle strokeStyle)
{
    m_impl->drawRect(rect, pattern, strokeStyle);
}

void GraphicsContext::drawEllipse(Point center, float radiusX, float radiusY, Pattern *pattern, StrokeStyle strokeStyle)
{
    m_impl->drawEllipse(center, radiusX, radiusY, pattern, strokeStyle);
}

void GraphicsContext::drawRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern *pattern, StrokeStyle strokeStyle)
{
    m_impl->drawRoundedRect(rect, radiusX, radiusY, pattern, strokeStyle);
}
} // karin