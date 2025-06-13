#ifndef SRC_GRAPHICS_RESOURCES_PLATFORM_H
#define SRC_GRAPHICS_RESOURCES_PLATFORM_H

#include <memory>

#include <karin/graphics/resources/graphics_device.h>

#ifdef KARIN_PLATFORM_WINDOWS
#include "d2d/d2d_graphics_device.h"
#include "d2d/d2d_surface_impl.h"
#elifdef KARIN_PLATFORM_UNIX
#include "vulkan/vk_graphics_device.h"
#include "vulkan/vk_surface_manager.h"
#endif

namespace karin
{

inline std::unique_ptr<GraphicsDevice> createGraphicsDevice()
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<D2DGraphicsDevice>();
#elifdef KARIN_PLATFORM_UNIX
    return std::make_unique<VkGraphicsDevice>();
#endif

    return nullptr;
}

}

#endif //SRC_GRAPHICS_RESOURCES_PLATFORM_H
