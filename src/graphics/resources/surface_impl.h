#ifndef SRC_GRAPHICS_RESOURCES_SURFACE_IMPL_H
#define SRC_GRAPHICS_RESOURCES_SURFACE_IMPL_H

namespace karin {

class ISurfaceImpl
{
public:
    virtual ~ISurfaceImpl() = default;

    virtual void cleanUp() = 0;
    virtual void present() = 0;
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_SURFACE_IMPL_H
