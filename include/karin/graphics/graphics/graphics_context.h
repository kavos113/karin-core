#ifndef KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
#define KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
#include <memory>

#include <karin/common/geometry/rectangle.h>

namespace karin
{
class IRendererImpl;
class IGraphicsContextImpl;

/**
 * GraphicsContext provides basic drawing operations(e.g., drawRect).
 *
 * This class is only created by Renderer.
 */
class GraphicsContext
{
private:
    explicit GraphicsContext(IRendererImpl* impl);
    ~GraphicsContext();

    friend class Renderer;

public:
    void fillRect(Rectangle rect, float strokeWidth = 1.0f);

private:
    IRendererImpl* m_rendererImpl;

    std::unique_ptr<IGraphicsContextImpl> m_impl;
};

} // karin

#endif //KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
