#include "d2d_surface_impl.h"

#include <stdexcept>

namespace karin
{
D2DSurfaceImpl::D2DSurfaceImpl(D2DGraphicsDevice *device, HWND hwnd)
    : m_device(device), m_hwnd(hwnd)
{
    createSwapChain();
    acquireBackBuffer();
}

void D2DSurfaceImpl::cleanUp()
{
    m_backBuffer.Reset();
    m_swapChain.Reset();
}

void D2DSurfaceImpl::present()
{
    if (FAILED(m_swapChain->Present(1, 0)))
    {
        throw std::runtime_error("Failed to present swap chain");
    }
}

void D2DSurfaceImpl::resize(Size size)
{
    m_backBuffer.Reset();

    if (FAILED(m_swapChain->ResizeBuffers(
            0,
            static_cast<UINT>(size.width),
            static_cast<UINT>(size.height),
            DXGI_FORMAT_B8G8R8A8_UNORM,
            0)))
    {
        throw std::runtime_error("Failed to resize swap chain buffers");
    }

    acquireBackBuffer();
}

void D2DSurfaceImpl::createSwapChain()
{
    constexpr DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {
        .Width = 0,
        .Height = 0,
        .Format = DXGI_FORMAT_B8G8R8A8_UNORM,
        .SampleDesc = {1, 0}, // No multisampling
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 2,
        .Scaling = DXGI_SCALING_STRETCH,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
        .AlphaMode = DXGI_ALPHA_MODE_IGNORE,
        .Flags = 0
    };

    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    if (FAILED(m_device->dxgiDevice()->GetAdapter(&adapter)))
    {
        throw std::runtime_error("Failed to get DXGI adapter");
    }

    Microsoft::WRL::ComPtr<IDXGIFactory2> factory;
    if (FAILED(adapter->GetParent(IID_PPV_ARGS(&factory))))
    {
        throw std::runtime_error("Failed to create DXGI factory");
    }

    if (FAILED(factory->CreateSwapChainForHwnd(
            m_device->dxgiDevice().Get(),
            m_hwnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            &m_swapChain)))
    {
        throw std::runtime_error("Failed to create swap chain");
    }
}

void D2DSurfaceImpl::acquireBackBuffer()
{
    m_backBuffer.Reset();
    if (FAILED(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_backBuffer))))
    {
        throw std::runtime_error("Failed to get back buffer from swap chain");
    }
}
} // karin