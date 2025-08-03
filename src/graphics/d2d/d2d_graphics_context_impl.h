#ifndef SRC_GRAPHICS_GRAPHICS_D2D_D2D_GRAPHICS_CONTEXT_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_D2D_D2D_GRAPHICS_CONTEXT_IMPL_H

#include <d2d1_1.h>
#include <wrl/client.h>

#include <graphics_context_impl.h>

#include <karin/common/geometry/rectangle.h>
#include <karin/common/geometry/point.h>
#include <karin/common/color/pattern.h>
#include <karin/graphics/stroke_style.h>

#include "d2d_device_resources.h"

namespace karin
{
class D2DGraphicsContextImpl : public IGraphicsContextImpl
{
public:
    D2DGraphicsContextImpl(
        Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext,
        D2DDeviceResources* deviceResources
    );

    ~D2DGraphicsContextImpl() override = default;

    void fillRect(Rectangle rect, Pattern& pattern) override;
    void fillEllipse(Point center, float radiusX, float radiusY, Pattern& pattern) override;
    void fillRoundedRect(Rectangle rect, float radiusX, float radiusY, Pattern& pattern) override;
    void fillPath(const PathImpl& path, Pattern& pattern) override;

    void drawLine(Point start, Point end, Pattern& pattern, const StrokeStyle& strokeStyle) override;
    void drawRect(Rectangle rect, Pattern& pattern, const StrokeStyle& strokeStyle) override;
    void drawEllipse(
        Point center, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle
    ) override;
    void drawRoundedRect(
        Rectangle rect, float radiusX, float radiusY, Pattern& pattern, const StrokeStyle& strokeStyle
    ) override;
    void drawPath(const PathImpl& path, Pattern& pattern, const StrokeStyle& strokeStyle) override;

private:
    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;
    D2DDeviceResources* m_deviceResources;
};
} // karin

#endif //SRC_GRAPHICS_GRAPHICS_D2D_D2D_GRAPHICS_CONTEXT_IMPL_H
