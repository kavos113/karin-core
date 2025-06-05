#ifndef SRC_GRAPHICS_GRAPHICS_D2D_D2D_GRAPHICS_CONTEXT_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_D2D_D2D_GRAPHICS_CONTEXT_IMPL_H

#include <d2d1_1.h>
#include <wrl/client.h>

#include <graphics/graphics_context_impl.h>

#include <karin/common/rectangle.h>

namespace karin
{

class D2DGraphicsContextImpl : public IGraphicsContextImpl
{
public:
    D2DGraphicsContextImpl(Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext);
    ~D2DGraphicsContextImpl() override = default;

    void drawRect(Rectangle rect, float strokeWidth) override;

private:
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_D2D_D2D_GRAPHICS_CONTEXT_IMPL_H
