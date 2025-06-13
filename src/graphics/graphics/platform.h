#ifndef SRC_GRAPHICS_GRAPHICS_PLATFORM_H
#define SRC_GRAPHICS_GRAPHICS_PLATFORM_H

#include "renderer_impl.h"
#include "graphics_context_impl.h"

#include <karin/graphics/resources/graphics_device.h>
#include <karin/system/window.h>

#include <memory>



#ifdef KARIN_PLATFORM_WINDOWS
#include <resources/d2d/d2d_graphics_device.h>

#include "d2d/d2d_renderer_impl.h"
#include "d2d/d2d_graphics_context_impl.h"
#elifdef KARIN_PLATFORM_UNIX
#include <resources/vulkan/vk_graphics_device.h>

#include "vulkan/vk_renderer_impl.h"
#include "vulkan/vk_graphics_context_impl.h"
#endif

namespace karin
{
inline std::unique_ptr<IRendererImpl> createRendererImpl(GraphicsDevice* device, Window::NativeHandle handle)
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<D2DRendererImpl>(dynamic_cast<D2DGraphicsDevice*>(device), dynamic_cast<D2DSurfaceImpl*>(surface));
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

#endif //SRC_GRAPHICS_GRAPHICS_PLATFORM_H
