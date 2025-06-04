#ifndef SRC_GRAPHICS_RESOURCES_D2D_D2D_SURFACE_IMPL_H
#define SRC_GRAPHICS_RESOURCES_D2D_D2D_SURFACE_IMPL_H

#include <wrl.h>
#include <dxgi1_6.h>

#include <resources/surface_impl.h>

#include "d2d_graphics_device.h"

namespace karin {

class D2DSurfaceImpl : public ISurfaceImpl
{
public:
    D2DSurfaceImpl(D2DGraphicsDevice* device, HWND hwnd);
    ~D2DSurfaceImpl() override = default;

    void cleanUp() override;
    void present() override;
    void resize(Size size) override;

    Microsoft::WRL::ComPtr<IDXGISurface> backBuffer() const
    {
        return m_backBuffer;
    }

private:
    void createSwapChain();
    void acquireBackBuffer();

    D2DGraphicsDevice* m_device;
    HWND m_hwnd;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISurface> m_backBuffer;
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_D2D_D2D_SURFACE_IMPL_H
