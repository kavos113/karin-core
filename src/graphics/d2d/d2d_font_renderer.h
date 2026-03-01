#ifndef SRC_GRAPHICS_D2D_D2D_FONT_RENDERER_H
#define SRC_GRAPHICS_D2D_D2D_FONT_RENDERER_H

#include <d2d1_3.h>
#include <wrl/client.h>

#include "font_renderer_impl.h"
#include "d2d_device_resources.h"

namespace karin
{
class D2DFontRenderer : public IFontRendererImpl
{
public:
    explicit D2DFontRenderer(Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2dContext, D2DDeviceResources* deviceResources);
    ~D2DFontRenderer() override;

    void drawText(const TextBlob& text, Point start, Pattern& pattern, const Transform2D& transform) const override;

private:
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;
    D2DDeviceResources* m_deviceResources;
};
} // karin

#endif //SRC_GRAPHICS_D2D_D2D_FONT_RENDERER_H