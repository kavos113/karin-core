#include "d2d_path_impl.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <numbers>

namespace karin
{
D2DPathImpl::D2DPathImpl(Microsoft::WRL::ComPtr<ID2D1Factory1> factory)
    : m_factory(std::move(factory))
{
    HRESULT hr = m_factory->CreatePathGeometry(&m_pathGeometry);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create D2D path geometry");
    }
}

void D2DPathImpl::start(Point start)
{
    HRESULT hr = m_pathGeometry->Open(&m_geometrySink);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to open D2D geometry sink");
    }

    m_geometrySink->SetFillMode(D2D1_FILL_MODE_WINDING);
    m_geometrySink->BeginFigure(
        D2D1::Point2F(start.x, start.y),
        D2D1_FIGURE_BEGIN_FILLED
    );
    m_currentPoint = start;
}

void D2DPathImpl::lineTo(Point end)
{
    m_geometrySink->AddLine(
        D2D1::Point2F(end.x, end.y)
    );
    m_currentPoint = end;
}

void D2DPathImpl::arcTo(Point center, float radiusX, float radiusY, float startAngle, float endAngle)
{
    auto end = Point(
        center.x + radiusX * std::cos(endAngle),
        center.y + radiusY * std::sin(endAngle)
    );

    if (startAngle > endAngle)
    {
        endAngle += 2 * std::numbers::pi;
    }

    auto size = (endAngle - startAngle) > std::numbers::pi ?
        D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL;

    m_geometrySink->AddArc(
        D2D1::ArcSegment(
            D2D1::Point2F(end.x, end.y),
            D2D1::SizeF(radiusX, radiusY),
            0.0f, // rotation angle
            D2D1_SWEEP_DIRECTION_CLOCKWISE,
            size
        )
    );
}

void D2DPathImpl::close()
{
    m_geometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);
    HRESULT hr = m_geometrySink->Close();
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to close D2D geometry sink");
    }
    m_geometrySink.Reset();
}

Microsoft::WRL::ComPtr<ID2D1Geometry> D2DPathImpl::geometry()
{
    return m_pathGeometry;
}
} // karin