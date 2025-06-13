#ifndef SRC_GRAPHICS_RESOURCES_SURFACE_IMPL_H
#define SRC_GRAPHICS_RESOURCES_SURFACE_IMPL_H

#include <karin/common/geometry/size.h>

namespace karin {

class ISurfaceImpl
{
public:
    virtual ~ISurfaceImpl() = default;

    virtual void cleanUp() = 0;
    virtual void resize(Size size) = 0;
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_SURFACE_IMPL_H
