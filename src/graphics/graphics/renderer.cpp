#include <karin/graphics/graphics/renderer.h>

#include "resources/platform.h"
#include "renderer_impl.h"

namespace karin
{
Renderer::Renderer(GraphicsDevice* device, Window &window)
    : m_window(&window)
{
    m_surfaceImpl = createSurfaceImpl(device, window.handle());
}

void Renderer::addDrawCommand(std::function<void(GraphicsContext &)> command)
{
    m_drawCommands.push_back(std::move(command));
}

void Renderer::update() const
{
    m_window->setOnPaint([this]
    {
        m_impl->beginDraw();
        GraphicsContext context;

        for (const auto &command : m_drawCommands)
        {
            command(context);
        }

        m_impl->endDraw();
    });
}
} // karin