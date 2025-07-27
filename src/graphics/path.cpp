#include <karin/graphics/path.h>

#include "path_impl.h"

namespace karin
{
Path::Path()
{
    m_impl = std::make_unique<PathImpl>();
}

Path::~Path() = default;

void Path::start(Point start) const
{
    m_impl->start(start);
}

void Path::lineTo(Point end) const
{
    m_impl->lineTo(end);
}

void Path::arcTo(Point center, float radiusX, float radiusY, float startAngle, float endAngle) const
{
    m_impl->arcTo(center, radiusX, radiusY, startAngle, endAngle);
}

void Path::close() const
{
    m_impl->close();
}

PathImpl* Path::impl() const
{
    return m_impl.get();
}
} // karin
