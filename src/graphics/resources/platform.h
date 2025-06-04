#ifndef SRC_GRAPHICS_RESOURCES_PLATFORM_H
#define SRC_GRAPHICS_RESOURCES_PLATFORM_H

#include <memory>

#include <karin/graphics/resources/graphics_device.h>
#include "surface_impl.h"

#ifdef KARIN_PLATFORM_WINDOWS
#include "d2d/d2d_graphics_device.h"
#include "d2d/d2d_surface_impl.h"
#endif

namespace karin
{

inline std::unique_ptr<GraphicsDevice> createGraphicsDevice()
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<D2DGraphicsDevice>();
#endif

    return nullptr;
}

inline std::unique_ptr<ISurfaceImpl> createSurfaceImpl(GraphicsDevice* device, void* windowHandle)
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<D2DSurfaceImpl>(dynamic_cast<D2DGraphicsDevice*>(device), static_cast<HWND>(windowHandle));
#endif

    return nullptr;
}

}

#endif //SRC_GRAPHICS_RESOURCES_PLATFORM_H
