#include <karin/graphics/renderer.h>

#include "platform.h"
#include "renderer_impl.h"

#include <stb_image/stb_image.h>

#include <vector>
#include <stdexcept>
#include <cstring>

namespace karin
{
Renderer::Renderer(GraphicsDevice* device, Window* window, SystemFont* systemFont)
    : m_window(window)
{
    m_impl = createRendererImpl(device, window->handle(), systemFont);
}

Renderer::~Renderer() = default;

void Renderer::addDrawCommand(std::function < void(GraphicsContext &) > command)
{
    m_drawCommands.push_back(std::move(command));
}

void Renderer::update() const
{
    m_window->setOnPaint(
        [this]
        {
            bool res = m_impl->beginDraw();
            if (!res)
            {
                return false;
            }

            GraphicsContext context(m_impl.get());

            for (const auto& command : m_drawCommands)
            {
                command(context);
            }

            m_impl->endDraw();

            return true;
        }
    );

    m_window->setOnResize(
        [this](Size size)
        {
            m_impl->resize(size);
        }
    );
}

void Renderer::cleanUp()
{
    m_impl->cleanUp();
}

void Renderer::setClearColor(const Color& color)
{
    m_impl->setClearColor(color);
}

Image Renderer::createImage(const std::string& filePath)
{
    int width, height, channels;
    stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 4);
    if (!data)
    {
        throw std::runtime_error("Failed to load image: " + filePath);
    }

    std::vector<std::byte> imageData(width * height * 4);
    memcpy(imageData.data(), data, imageData.size());

    return m_impl->createImage(imageData, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
}

Image Renderer::createImage(const std::vector<std::byte>& data, uint32_t width, uint32_t height)
{
    return m_impl->createImage(data, width, height);
}
} // karin