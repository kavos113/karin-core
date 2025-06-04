#include "d2d_graphics_device_impl.h"

#include <array>
#include <d3d11.h>
#include <stdexcept>

namespace karin
{
D2DGraphicsDeviceImpl::D2DGraphicsDeviceImpl()
{
    createD2DFactory();
    createDXGIDevice();
    createD2DDevice();
}

void D2DGraphicsDeviceImpl::cleanUp()
{
    m_d2dDevice.Reset();
    m_d2dFactory.Reset();
    m_dxgiDevice.Reset();
}

void D2DGraphicsDeviceImpl::createD2DFactory()
{
    if (FAILED(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            IID_PPV_ARGS(&m_d2dFactory))))
    {
        throw std::runtime_error("Failed to create D2D factory");
    }
}

void D2DGraphicsDeviceImpl::createD2DDevice()
{
    if (FAILED(m_d2dFactory->CreateDevice(
            m_dxgiDevice.Get(),
            &m_d2dDevice)))
    {
        throw std::runtime_error("Failed to create D2D device");
    }
}

void D2DGraphicsDeviceImpl::createDXGIDevice()
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

    if (FAILED(D3D11CreateDevice(
            nullptr, // Use default adapter
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr, // No software device
            flags,
            featureLevels.data(),
            featureLevels.size(),
            D3D11_SDK_VERSION,
            &d3dDevice,
            nullptr, // Feature level not needed
            &d3dContext)))
    {
        throw std::runtime_error("Failed to create D3D11 device");
    }

    if (FAILED(d3dDevice.As(&m_dxgiDevice)))
    {
        throw std::runtime_error("Failed to get DXGI device from D3D11 device");
    }
}
} // karin