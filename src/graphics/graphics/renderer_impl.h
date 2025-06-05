#ifndef SRC_GRAPHICS_GRAPHICS_RENDERER_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_RENDERER_IMPL_H

#include <karin/common/geometry/size.h>

namespace karin {

class IRendererImpl
{
public:
    virtual ~IRendererImpl() = default;

    virtual void cleanUp() = 0;

    virtual void beginDraw() = 0;
    virtual void endDraw() = 0;
    virtual void resize(Size size) = 0;
    virtual void reset() = 0;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_RENDERER_IMPL_H
