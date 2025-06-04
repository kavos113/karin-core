#ifndef SRC_GRAPHICS_RESOURCES_GRAPHICS_DEVICE_IMPL_H
#define SRC_GRAPHICS_RESOURCES_GRAPHICS_DEVICE_IMPL_H

namespace karin
{
class IGraphicsDeviceImpl
{
public:
    virtual ~IGraphicsDeviceImpl() = default;

    virtual void cleanUp() = 0;
};
}

#endif //SRC_GRAPHICS_RESOURCES_GRAPHICS_DEVICE_IMPL_H
