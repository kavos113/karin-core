#ifndef SRC_GRAPHICS_RESOURCES_D2D_D2D_SURFACE_IMPL_H
#define SRC_GRAPHICS_RESOURCES_D2D_D2D_SURFACE_IMPL_H

#include <wrl.h>
#include <dxgi1_6.h>

#include <resources/surface_impl.h>

#include "d2d_graphics_device_impl.h"

namespace karin {

class D2DSurfaceImpl : public ISurfaceImpl
{
public:
    D2DSurfaceImpl(D2DGraphicsDeviceImpl* device, HWND hwnd);
    ~D2DSurfaceImpl() override = default;

    void cleanUp() override;

    D2D1_BITMAP_PROPERTIES1 bitmapProperties() const
    {
        return D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            DEFAULT_DPI, DEFAULT_DPI);
    }

private:
    void createSwapChain();
    void acquireBackBuffer();

    D2DGraphicsDeviceImpl* m_device;
    HWND m_hwnd;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISurface> m_backBuffer;

    static constexpr float DEFAULT_DPI = 96.0f; // Default DPI for D2D
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_D2D_D2D_SURFACE_IMPL_H
