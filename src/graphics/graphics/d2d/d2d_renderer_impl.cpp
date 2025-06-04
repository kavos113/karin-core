#include "d2d_renderer_impl.h"

#include <stdexcept>

namespace karin
{
D2DRendererImpl::D2DRendererImpl(D2DGraphicsDevice *device, D2DSurfaceImpl *surface)
    : m_device(device), m_surface(surface)
{
    if (FAILED(m_device->device()->CreateDeviceContext(
        D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_deviceContext)))
    {
        throw std::runtime_error("Failed to create D2D device context");
    }

    Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;
    if (FAILED(m_deviceContext->CreateBitmapFromDxgiSurface(
        m_surface->backBuffer().Get(),
        bitmapProperties,
        &bitmap)))
    {
        throw std::runtime_error("Failed to create D2D bitmap from DXGI surface");
    }

    m_deviceContext->SetTarget(bitmap.Get());
}

void D2DRendererImpl::cleanUp()
{
    m_deviceContext.Reset();
    m_surface = nullptr;
    m_device = nullptr;
}

void D2DRendererImpl::beginDraw()
{
    m_deviceContext->BeginDraw();
    m_deviceContext->Clear(m_clearColor);
}

void D2DRendererImpl::endDraw()
{
    if (FAILED(m_deviceContext->EndDraw()))
    {
        throw std::runtime_error("Failed to end D2D drawing");
    }

    m_surface->present();
}
} // karin