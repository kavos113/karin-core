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

void GraphicsContext::fillRect(Rectangle rect, float strokeWidth)
{
}
} // karin