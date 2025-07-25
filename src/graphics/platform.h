#ifndef SRC_GRAPHICS_RESOURCES_PLATFORM_H
#define SRC_GRAPHICS_RESOURCES_PLATFORM_H

#include <memory>

#include <karin/graphics/graphics_device.h>

#ifdef KARIN_PLATFORM_WINDOWS
#include "d2d/d2d_graphics_device.h"
#include "d2d/d2d_renderer_impl.h"
#include "d2d/d2d_graphics_context_impl.h"
#elifdef KARIN_PLATFORM_UNIX
#include "vulkan/vk_graphics_device.h"
#include "vulkan/vk_surface_manager.h"
#include "vulkan/vk_renderer_impl.h"
#include "vulkan/vk_graphics_context_impl.h"
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

inline std::unique_ptr<IRendererImpl> createRendererImpl(GraphicsDevice* device, Window::NativeHandle handle)
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<D2DRendererImpl>(dynamic_cast<D2DGraphicsDevice*>(device), static_cast<HWND>(handle.hwnd));
#elifdef KARIN_PLATFORM_UNIX
    return std::make_unique<VkRendererImpl>(dynamic_cast<VkGraphicsDevice*>(device), handle.window, reinterpret_cast<Display*>(handle.display));
#endif
    return nullptr;
}

inline std::unique_ptr<IGraphicsContextImpl> createGraphicsContextImpl(IRendererImpl* impl)
{
#ifdef KARIN_PLATFORM_WINDOWS
    auto d2dImpl = dynamic_cast<D2DRendererImpl*>(impl);
    return std::make_unique<D2DGraphicsContextImpl>(
        d2dImpl->deviceContext(),
        d2dImpl->deviceResources()
    );
#elifdef KARIN_PLATFORM_UNIX
    return std::make_unique<VkGraphicsContextImpl>(
        dynamic_cast<VkRendererImpl*>(impl)
    );
#endif

    return nullptr;
}

}

#endif //SRC_GRAPHICS_RESOURCES_PLATFORM_H
