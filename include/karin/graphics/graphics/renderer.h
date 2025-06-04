#ifndef KARIN_GRAPHICS_GRAPHICS_RENDERER_H
#define KARIN_GRAPHICS_GRAPHICS_RENDERER_H
#include <vector>
#include <functional>
#include <memory>

#include <karin/graphics/resources/graphics_device.h>
#include <karin/system/window.h>

#include "graphics_context.h"

namespace karin
{

class ISurfaceImpl;
class IRendererImpl;

class Renderer
{
public:
    Renderer(GraphicsDevice *device, Window *window);
    ~Renderer();

    void addDrawCommand(std::function<void(GraphicsContext&)> command);
    void update() const;

    void cleanUp();

private:
    Window* m_window;

    std::vector<std::function<void(GraphicsContext&)>> m_drawCommands;

    std::unique_ptr<IRendererImpl> m_impl;
    std::unique_ptr<ISurfaceImpl> m_surfaceImpl;
};

} // karin

#endif //KARIN_GRAPHICS_GRAPHICS_RENDERER_H
