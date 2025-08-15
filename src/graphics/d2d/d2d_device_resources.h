#ifndef SRC_GRAPHICS_GRAPHICS_D2D_DEVICE_RESOURCES_H
#define SRC_GRAPHICS_GRAPHICS_D2D_DEVICE_RESOURCES_H
#include <d2d1_1.h>
#include <map>
#include <unordered_map>
#include <wrl/client.h>

#include <karin/graphics/stroke_style.h>
#include <karin/graphics/pattern.h>
#include <karin/graphics/solid_color_pattern.h>
#include <karin/graphics/linear_gradient_pattern.h>

#include <path_impl.h>

namespace karin
{
class D2DDeviceResources
{
public:
    explicit D2DDeviceResources(
        Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext,
        Microsoft::WRL::ComPtr<ID2D1Factory1> factory
    )
        : m_deviceContext(std::move(deviceContext)),
          m_factory(std::move(factory))
    {
    }

    ~D2DDeviceResources() = default;

    void clear();

    Microsoft::WRL::ComPtr<ID2D1Brush> brush(Pattern& pattern);
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> solidColorBrush(const SolidColorPattern& pattern);
    Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush> linearGradientBrush(const LinearGradientPattern& pattern);
    Microsoft::WRL::ComPtr<ID2D1StrokeStyle> strokeStyle(const StrokeStyle& style);
    Microsoft::WRL::ComPtr<ID2D1PathGeometry> pathGeometry(const PathImpl& path);

private:
    static D2D1_CAP_STYLE toD2DCapStyle(StrokeStyle::CapStyle capStyle);
    static D2D1_LINE_JOIN toD2DJoinStyle(StrokeStyle::JoinStyle joinStyle);

    // TODO: create before starting draw calls?
    std::unordered_map<size_t, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> m_solidColorBrushes;
    std::unordered_map<size_t, Microsoft::WRL::ComPtr<ID2D1LinearGradientBrush>> m_linearGradientBrushes;
    std::map<StrokeStyle, Microsoft::WRL::ComPtr<ID2D1StrokeStyle>> m_strokeStyles;
    std::map<uint32_t, Microsoft::WRL::ComPtr<ID2D1PathGeometry>> m_pathGeometries;

    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;
    Microsoft::WRL::ComPtr<ID2D1Factory1> m_factory;
};
} // karin

#endif //SRC_GRAPHICS_GRAPHICS_D2D_DEVICE_RESOURCES_H
