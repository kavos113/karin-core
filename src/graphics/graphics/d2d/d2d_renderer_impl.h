#ifndef SRC_GRAPHICS_GRAPHICS_D2D_D2D_RENDERER_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_D2D_D2D_RENDERER_IMPL_H

#include <d2d1_1.h>

#include <graphics/renderer_impl.h>

#include "resources/d2d/d2d_graphics_device.h"
#include "resources/d2d/d2d_surface_impl.h"

namespace karin {

class D2DRendererImpl : public IRendererImpl
{
public:
    D2DRendererImpl(D2DGraphicsDevice* device, D2DSurfaceImpl* surface);
    ~D2DRendererImpl() override = default;

    void cleanUp() override;

    void beginDraw() override;
    void endDraw() override;
    void resize(Size size) override;
    void reset() override;

private:
    void setTargetBitmap();

    // weak reference
    D2DGraphicsDevice* m_device;
    D2DSurfaceImpl* m_surface;

    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;

    const D2D1_COLOR_F m_clearColor = D2D1::ColorF(D2D1::ColorF::Black);

    static constexpr float DEFAULT_DPI = 96.0f; // Default DPI for D2D
    const D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            DEFAULT_DPI, DEFAULT_DPI);
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_D2D_D2D_RENDERER_IMPL_H
