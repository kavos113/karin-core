#include <karin/graphics/graphics/path.h>

#include "path_impl.h"

namespace karin
{
Path::Path()
{
    m_impl = std::make_unique<PathImpl>();
}

void Path::start(Point start)
{
    m_impl->start(start);
}

void Path::lineTo(Point end)
{
    m_impl->lineTo(end);
}

void Path::arcTo(Point center, float radiusX, float radiusY, float startAngle, float endAngle)
{
    m_impl->arcTo(center, radiusX, radiusY, startAngle, endAngle);
}

void Path::close()
{
    m_impl->close();
}
} // karin