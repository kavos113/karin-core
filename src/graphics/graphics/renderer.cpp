#include <karin/graphics/graphics/renderer.h>

#include "platform.h"
#include "resources/platform.h"
#include "renderer_impl.h"

namespace karin
{
Renderer::Renderer(GraphicsDevice* device, Window* window)
    : m_window(window)
{
    m_surfaceImpl = createSurfaceImpl(device, window->handle());
    m_impl = createRendererImpl(device, m_surfaceImpl.get());
}

Renderer::~Renderer() = default;

void Renderer::addDrawCommand(std::function<void(GraphicsContext &)> command)
{
    m_drawCommands.push_back(std::move(command));
}

void Renderer::update() const
{
    m_window->setOnPaint([this]
    {
        m_surfaceImpl->beforeFrame();
        m_impl->beginDraw();

        GraphicsContext context(m_impl.get());

        for (const auto &command : m_drawCommands)
        {
            command(context);
        }

        m_impl->endDraw();
        m_surfaceImpl->present();
    });

    m_window->setOnResize([this](Size size)
    {
        m_impl->reset();
        m_surfaceImpl->resize(size);
        m_impl->resize(size);
    });
}

void Renderer::cleanUp()
{
    m_impl->cleanUp();
    m_surfaceImpl->cleanUp();
}
} // karin