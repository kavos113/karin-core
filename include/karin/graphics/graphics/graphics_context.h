#ifndef KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
#define KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
#include <memory>

#include "karin/common/rectangle.h"

namespace karin {

class IGraphicsContextImpl;

/**
 * GraphicsContext provides basic drawing operations(e.g., drawRect).
 *
 * This class is only created by Renderer.
 */
class GraphicsContext
{
private:
    class IRendererImpl;

    explicit GraphicsContext(IRendererImpl* impl);
    ~GraphicsContext() = default;

    friend class Renderer;

public:
    void drawRect(Rectangle rect, float strokeWidth = 1.0f);

private:
    IRendererImpl* m_rendererImpl;

    std::unique_ptr<IGraphicsContextImpl> m_impl;
};

} // karin

#endif //KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
