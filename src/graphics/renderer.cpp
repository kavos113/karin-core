#include <karin/graphics/renderer.h>

#include "platform.h"
#include "renderer_impl.h"

namespace karin
{
Renderer::Renderer(GraphicsDevice* device, Window* window)
    : m_window(window)
{
    m_impl = createRendererImpl(device, window->handle());
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
        bool res = m_impl->beginDraw();
        if (!res)
        {
            return false;
        }

        GraphicsContext context(m_impl.get());

        for (const auto &command : m_drawCommands)
        {
            command(context);
        }

        m_impl->endDraw();

        return true;
    });

    m_window->setOnResize([this](Size size)
    {
        m_impl->resize(size);
    });
}

void Renderer::cleanUp()
{
    m_impl->cleanUp();
}

void Renderer::setClearColor(const Color &color)
{
    m_impl->setClearColor(color);
}
} // karin