#ifndef SRC_GRAPHICS_GRAPHICS_D2D_D2D_GRAPHICS_CONTEXT_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_D2D_D2D_GRAPHICS_CONTEXT_IMPL_H

#include <d2d1_1.h>
#include <wrl/client.h>

#include <graphics/graphics_context_impl.h>

#include <karin/common/geometry/rectangle.h>

#include "d2d_device_resources.h"

namespace karin
{

class D2DGraphicsContextImpl : public IGraphicsContextImpl
{
public:
    D2DGraphicsContextImpl(Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext,
                           D2DDeviceResources* deviceResources);

    ~D2DGraphicsContextImpl() override = default;

    void fillRect(Rectangle rect, Pattern pattern, float strokeWidth) override;

private:
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;
    D2DDeviceResources* m_deviceResources;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_D2D_D2D_GRAPHICS_CONTEXT_IMPL_H
