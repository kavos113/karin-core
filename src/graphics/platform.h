#ifndef SRC_GRAPHICS_RESOURCES_PLATFORM_H
#define SRC_GRAPHICS_RESOURCES_PLATFORM_H

#include <memory>

#include <karin/graphics/graphics_device.h>
#include <karin/system/window.h>
#include <karin/system/system_font.h>

#ifdef KARIN_PLATFORM_DIRECTX
#include "d2d/d2d_renderer_impl.h"
#include "d2d/d2d_graphics_context_impl.h"
#elifdef KARIN_PLATFORM_VULKAN
#include "vulkan/vulkan_graphics_device.h"
#include "vulkan/vulkan_renderer_impl.h"
#include "vulkan/vulkan_graphics_context_impl.h"
#include "text/font_loader.h"
#endif

namespace karin
{
inline std::unique_ptr<GraphicsDevice> createGraphicsDevice()
{
#ifdef KARIN_PLATFORM_DIRECTX
    return nullptr;
#elifdef KARIN_PLATFORM_VULKAN
    return std::make_unique<VulkanGraphicsDevice>();
#endif

    return nullptr;
}

inline std::unique_ptr<IRendererImpl> createRendererImpl(
    GraphicsDevice* device, Window::NativeHandle handle, SystemFont* systemFont
)
{
#ifdef KARIN_PLATFORM_DIRECTX
    return std::make_unique<D2DRendererImpl>(static_cast<HWND>(handle.hwnd));
#elifdef KARIN_PLATFORM_VULKAN
    return std::make_unique<VulkanRendererImpl>(
        dynamic_cast<VulkanGraphicsDevice*>(device), handle, std::make_unique<FontLoader>(systemFont)
    );
#endif
    return nullptr;
}

inline std::unique_ptr<IGraphicsContextImpl> createGraphicsContextImpl(IRendererImpl* impl)
{
#ifdef KARIN_PLATFORM_DIRECTX
    auto d2dImpl = dynamic_cast<D2DRendererImpl*>(impl);
    return std::make_unique<D2DGraphicsContextImpl>(
        d2dImpl->deviceContext(),
        d2dImpl->deviceResources()
    );
#elifdef KARIN_PLATFORM_VULKAN
    return std::make_unique<VulkanGraphicsContextImpl>(
        dynamic_cast<VulkanRendererImpl*>(impl)
    );
#endif

    return nullptr;
}
}

#endif //SRC_GRAPHICS_RESOURCES_PLATFORM_H