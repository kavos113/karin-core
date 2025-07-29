#ifndef SRC_GRAPHICS_GRAPHICS_RENDERER_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_RENDERER_IMPL_H

#include <karin/common/geometry/size.h>
#include <karin/common/color/color.h>

namespace karin
{
class IRendererImpl
{
public:
    virtual ~IRendererImpl() = default;

    virtual void cleanUp() = 0;

    // return false if the draw operation should be skipped
    virtual bool beginDraw() = 0;
    virtual void endDraw() = 0;
    virtual void resize(Size size) = 0;
    virtual void setClearColor(const Color& color) = 0;
};
} // karin

#endif //SRC_GRAPHICS_GRAPHICS_RENDERER_IMPL_H
