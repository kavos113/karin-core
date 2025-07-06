#ifndef SRC_GRAPHICS_GRAPHICS_D2D_D2D_PATH_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_D2D_D2D_PATH_IMPL_H

#include <d2d1_1.h>
#include <wrl/client.h>

#include <graphics/path_impl.h>

namespace karin
{

class D2DPathImpl : public IPathImpl
{
public:
    explicit D2DPathImpl(Microsoft::WRL::ComPtr<ID2D1Factory1> factory);
    ~D2DPathImpl() override = default;

    void start(Point start) override;
    void lineTo(Point end) override;
    void arcTo(
        Point center,
        float radiusX,
        float radiusY,
        float startAngle,
        float endAngle
    ) override;
    void close() override;

    Microsoft::WRL::ComPtr<ID2D1Geometry> geometry();

private:
    Microsoft::WRL::ComPtr<ID2D1PathGeometry> m_pathGeometry;
    Microsoft::WRL::ComPtr<ID2D1GeometrySink> m_geometrySink;

    Microsoft::WRL::ComPtr<ID2D1Factory1> m_factory;

    Point m_currentPoint{};
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_D2D_D2D_PATH_IMPL_H
