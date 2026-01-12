#include "d2d_context.h"

#include <array>
#include <d3d11.h>
#include <stdexcept>

namespace karin
{
D2DContext& D2DContext::instance()
{
    static D2DContext instance;
    return instance;
}

void D2DContext::init()
{
    createD2DFactory();
    createDXGIDevice();
    createD2DDevice();
    createDWriteFactory();
}

void D2DContext::createD2DFactory()
{
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&m_d2dFactory));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D2D Factory");
    }
}

void D2DContext::createD2DDevice()
{
    HRESULT hr = m_d2dFactory->CreateDevice(m_dxgiDevice.Get(), &m_d2dDevice);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D2D device");
    }
}

void D2DContext::createDXGIDevice()
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

    HRESULT hr = D3D11CreateDevice(
        nullptr, // default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr, // no software device
        flags,
        featureLevels.data(),
        featureLevels.size(),
        D3D11_SDK_VERSION,
        &d3dDevice,
        nullptr, // feature level not needed,
        &d3dContext
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D3D11 device");
    }

    hr = d3dDevice.As(&m_dxgiDevice);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to get DXGI device from D3D11 device");
    }
}

void D2DContext::createDWriteFactory()
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
} // karin