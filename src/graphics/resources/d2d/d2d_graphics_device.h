#ifndef SRC_GRAPHICS_RESOURCES_D2D_D2D_GRAPHICS_DEVICE_IMPL_H
#define SRC_GRAPHICS_RESOURCES_D2D_D2D_GRAPHICS_DEVICE_IMPL_H

#include <d2d1_1.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include <karin/graphics/resources/graphics_device.h>


namespace karin
{

class D2DGraphicsDevice : public GraphicsDevice
{
public:
    D2DGraphicsDevice();
    ~D2DGraphicsDevice() override = default;

    void cleanUp() override;

    Microsoft::WRL::ComPtr<ID2D1Factory1>& factory();
    Microsoft::WRL::ComPtr<ID2D1Device>& device();
    Microsoft::WRL::ComPtr<IDXGIDevice4>& dxgiDevice();

private:
    void createD2DFactory();
    void createD2DDevice();
    void createDXGIDevice();

    Microsoft::WRL::ComPtr<ID2D1Factory1> m_d2dFactory;
    Microsoft::WRL::ComPtr<ID2D1Device> m_d2dDevice;
    Microsoft::WRL::ComPtr<IDXGIDevice4> m_dxgiDevice;
};

} // karin

#endif //SRC_GRAPHICS_RESOURCES_D2D_D2D_GRAPHICS_DEVICE_IMPL_H
