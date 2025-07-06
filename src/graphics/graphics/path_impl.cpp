#include "path_impl.h"

namespace karin
{
void PathImpl::start(Point start)
{
    m_commands.clear();
    m_currentPoint = start;
    m_startPoint = start;
}

void PathImpl::lineTo(Point end)
{
    m_commands.push_back(LineArgs{m_currentPoint, end});
    m_currentPoint = end;
}

void PathImpl::arcTo(Point center, float radiusX, float radiusY, float startAngle, float endAngle)
{
    m_commands.push_back(ArcArgs{center, radiusX, radiusY, startAngle, endAngle});
    m_currentPoint = Point(
        center.x + radiusX * cos(endAngle),
        center.y + radiusY * sin(endAngle)
    );
}

void PathImpl::close()
{
    if (m_currentPoint != m_startPoint)
    {
        m_commands.push_back(LineArgs{m_currentPoint, m_startPoint});
        m_currentPoint = m_startPoint;
    }
}
} // karin