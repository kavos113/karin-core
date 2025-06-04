#include <karin/graphics/graphics/renderer.h>

#include "resources/platform.h"

namespace karin
{
Renderer::Renderer(GraphicsDevice* device, Window &window)
    : m_window(&window),
      m_impl(createRendererImpl())
{
    m_surfaceImpl = createSurfaceImpl(device, window.handle());
}

void Renderer::addDrawCommand(std::function<void(GraphicsContext &)> command)
{
}

void Renderer::update()
{
}
} // karin