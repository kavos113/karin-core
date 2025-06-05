#ifndef SRC_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_IMPL_H
#include <karin/common/geometry/rectangle.h>
#include <karin/common/color/pattern.h>

namespace karin
{

class IGraphicsContextImpl
{
public:
    virtual ~IGraphicsContextImpl() = default;

    virtual void fillRect(Rectangle rect, Pattern pattern, float strokeWidth) = 0;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_IMPL_H
