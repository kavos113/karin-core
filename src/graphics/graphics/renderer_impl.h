#ifndef SRC_GRAPHICS_GRAPHICS_RENDERER_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_RENDERER_IMPL_H

namespace karin {

class IRendererImpl
{
public:
    virtual ~IRendererImpl() = default;

    virtual void cleanUp() = 0;

    virtual void beginDraw() = 0;
    virtual void endDraw() = 0;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_RENDERER_IMPL_H
