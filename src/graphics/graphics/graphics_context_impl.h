#ifndef SRC_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_IMPL_H
#include "karin/common/rectangle.h"

namespace karin {

class IGraphicsContextImpl
{
public:
    virtual ~IGraphicsContextImpl() = default;

    virtual void drawRect(Rectangle rect, float strokeWidth) = 0;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_IMPL_H
