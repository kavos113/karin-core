#ifndef SRC_GRAPHICS_GRAPHICS_D2D_DEVICE_RESOURCES_H
#define SRC_GRAPHICS_GRAPHICS_D2D_DEVICE_RESOURCES_H
#include <d2d1_1.h>
#include <map>
#include <wrl/client.h>

#include <karin/common/color/solid_color_pattern.h>

namespace karin
{

class D2DDeviceResources
{
public:
    explicit D2DDeviceResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext)
        : m_deviceContext(std::move(deviceContext)) {}
    ~D2DDeviceResources() = default;

    void clear();

    Microsoft::WRL::ComPtr<ID2D1Brush> brush(Pattern *pattern);
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> solidColorBrush(const SolidColorPattern& pattern);

private:
    // TODO: create before starting draw calls?
    std::map<SolidColorPattern, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> m_solidColorBrushes;

    Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_deviceContext;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_D2D_DEVICE_RESOURCES_H
