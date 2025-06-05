#ifndef SRC_GRAPHICS_GRAPHICS_PLATFORM_H
#define SRC_GRAPHICS_GRAPHICS_PLATFORM_H

#include "renderer_impl.h"
#include "graphics_context_impl.h"

#include <memory>


#ifdef KARIN_PLATFORM_WINDOWS
#include <resources/d2d/d2d_graphics_device.h>

#include "d2d/d2d_renderer_impl.h"
#include "d2d/d2d_graphics_context_impl.h"
#endif

namespace karin
{
inline std::unique_ptr<IRendererImpl> createRendererImpl(GraphicsDevice* device, ISurfaceImpl* surface)
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<D2DRendererImpl>(dynamic_cast<D2DGraphicsDevice*>(device), dynamic_cast<D2DSurfaceImpl*>(surface));
#endif
    return nullptr;
}

inline std::unique_ptr<IGraphicsContextImpl> createGraphicsContextImpl(IRendererImpl* impl)
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<D2DGraphicsContextImpl>(dynamic_cast<D2DRendererImpl*>(impl)->deviceContext());
#endif

    return nullptr;
}
}

#endif //SRC_GRAPHICS_GRAPHICS_PLATFORM_H
