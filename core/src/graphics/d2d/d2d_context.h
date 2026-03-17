#ifndef SRC_GRAPHICS_D2D_D2D_CONTEXT_H
#define SRC_GRAPHICS_D2D_D2D_CONTEXT_H

#include <d2d1_1.h>
#include <dxgi1_6.h>
#include <dwrite.h>
#include <wrl.h>

namespace karin
{
class D2DContext
{
public:
    static D2DContext& instance();

    Microsoft::WRL::ComPtr<ID2D1Factory1>& factory()
    {
        return m_d2dFactory;
    }

    Microsoft::WRL::ComPtr<ID2D1Device>& device()
    {
        return m_d2dDevice;
    }

    Microsoft::WRL::ComPtr<IDXGIDevice4>& dxgiDevice()
    {
        return m_dxgiDevice;
    }

    Microsoft::WRL::ComPtr<IDWriteFactory>& dwriteFactory()
    {
        return m_dwriteFactory;
    }

private:
    D2DContext();
    ~D2DContext() = default;

    void createD2DFactory();
    void createD2DDevice();
    void createDXGIDevice();
    void createDWriteFactory();

    Microsoft::WRL::ComPtr<ID2D1Factory1> m_d2dFactory;
    Microsoft::WRL::ComPtr<ID2D1Device> m_d2dDevice;
    Microsoft::WRL::ComPtr<IDXGIDevice4> m_dxgiDevice;

    Microsoft::WRL::ComPtr<IDWriteFactory> m_dwriteFactory;
};
} // karin

#endif //SRC_GRAPHICS_D2D_D2D_CONTEXT_H