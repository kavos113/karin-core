#include "d2d_graphics_device.h"

#include <array>
#include <d3d11.h>
#include <stdexcept>

namespace karin
{
D2DGraphicsDevice::D2DGraphicsDevice()
{
    createD2DFactory();
    createDXGIDevice();
    createD2DDevice();
    createDWriteFactory();
}

void D2DGraphicsDevice::cleanUp()
{
    m_dwriteFactory.Reset();
    m_d2dDevice.Reset();
    m_d2dFactory.Reset();
    m_dxgiDevice.Reset();
}

void D2DGraphicsDevice::createD2DFactory()
{
    if (FAILED(
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            IID_PPV_ARGS(&m_d2dFactory)
        )
    ))
    {
        throw std::runtime_error("Failed to create D2D factory");
    }
}

void D2DGraphicsDevice::createD2DDevice()
{
    if (FAILED(
        m_d2dFactory->CreateDevice(
            m_dxgiDevice.Get(),
            &m_d2dDevice
        )
    ))
    {
        throw std::runtime_error("Failed to create D2D device");
    }
}

void D2DGraphicsDevice::createDXGIDevice()
{
    constexpr UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    std::array featureLevels = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext;

    if (FAILED(
        D3D11CreateDevice(
            nullptr, // Use default adapter
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr, // No software device
            flags,
            featureLevels.data(),
            featureLevels.size(),
            D3D11_SDK_VERSION,
            &d3dDevice,
            nullptr, // Feature level not needed
            &d3dContext
        )
    ))
    {
        throw std::runtime_error("Failed to create D3D11 device");
    }

    if (FAILED(d3dDevice.As(&m_dxgiDevice)))
    {
        throw std::runtime_error("Failed to get DXGI device from D3D11 device");
    }
}

void D2DGraphicsDevice::createDWriteFactory()
{
    HRESULT hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        &m_dwriteFactory
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create dwrite factory");
    }
}

Microsoft::WRL::ComPtr<ID2D1Factory1>& D2DGraphicsDevice::factory()
{
    return m_d2dFactory;
}

Microsoft::WRL::ComPtr<ID2D1Device>& D2DGraphicsDevice::device()
{
    return m_d2dDevice;
}

Microsoft::WRL::ComPtr<IDXGIDevice4>& D2DGraphicsDevice::dxgiDevice()
{
    return m_dxgiDevice;
}

Microsoft::WRL::ComPtr<IDWriteFactory>& D2DGraphicsDevice::dwriteFactory()
{
    return m_dwriteFactory;
}
} // karin