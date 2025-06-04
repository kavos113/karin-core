#ifndef SRC_GRAPHICS_RESOURCES_PLATFORM_H
#define SRC_GRAPHICS_RESOURCES_PLATFORM_H

#include <memory>

#include "graphics_device_impl.h"

#ifdef KARIN_PLATFORM_WINDOWS
#include "d2d/d2d_graphics_device_impl.h"
#endif

namespace karin
{

inline std::unique_ptr<IGraphicsDeviceImpl> createGraphicsDeviceImpl()
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<D2DGraphicsDeviceImpl>();
#endif

    return nullptr;
}

}

#endif //SRC_GRAPHICS_RESOURCES_PLATFORM_H
