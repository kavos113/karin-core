#ifndef KARIN_GRAPHICS_GRAPHICS_RENDERER_H
#define KARIN_GRAPHICS_GRAPHICS_RENDERER_H
#include <vector>
#include <functional>
#include <memory>

#include <karin/graphics/resources/graphics_device.h>
#include <karin/system/window.h>
#include <karin/common/color/color.h>

#include "graphics_context.h"

namespace karin
{

class IRendererImpl;

/**
 * Renderer manages window surface(includes swapchain) of low-level graphics API(D2D -> ID2D1GraphicsContext, Vulkan -> VkSurface).
 *
 * It also has a list of draw commands that can be executed in the rendering loop.
 */
class Renderer
{
public:
    Renderer(GraphicsDevice *device, Window *window);
    ~Renderer();

    /**
     * Add a rendering command to the renderer.
     *
     * The order of the commands is guaranteed to be executed in the order they are added,
     * but if the accuracy of the order is important, you should use RenderPass class.
     *
     * example:
     * <pre>
     * addDrawCommand([](GraphicsContext &gc) {
     *     gc.drawRect(Rectangle(0, 0, 100, 100));
     * });
     * </pre>
     *
     * @param command rendering command. use GraphicsContext to draw.
     */
    void addDrawCommand(std::function<void(GraphicsContext&)> command);
    void update() const;
    void setClearColor(const Color& color);

    void cleanUp();

private:
    Window* m_window;

    std::vector<std::function<void(GraphicsContext&)>> m_drawCommands;

    std::unique_ptr<IRendererImpl> m_impl;
};

} // karin

#endif //KARIN_GRAPHICS_GRAPHICS_RENDERER_H
