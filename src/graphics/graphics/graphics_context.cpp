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

void GraphicsContext::fillRect(Rectangle rect, Pattern *pattern, float strokeWidth)
{
    m_impl->fillRect(rect, pattern, strokeWidth);
}

} // karin