#include <karin/graphics/graphics/graphics_context.h>

namespace karin
{
GraphicsContext::GraphicsContext(IRendererImpl *impl)
    : m_rendererImpl(impl)
    , m_impl(nullptr) // This should be initialized with a concrete implementation
{
}

void GraphicsContext::fillRect(Rectangle rect, float strokeWidth)
{
}
} // karin