#include "graphics/path_impl.h"

#include <cmath>

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
    m_commands.emplace_back(LineArgs{end});
    m_currentPoint = end;
}

void PathImpl::arcTo(Point center, float radiusX, float radiusY, float startAngle, float endAngle)
{
    m_commands.emplace_back(ArcArgs{center, radiusX, radiusY, startAngle, endAngle});
    m_currentPoint = Point(
        center.x + radiusX * std::cos(endAngle),
        center.y + radiusY * std::sin(endAngle)
    );
}

void PathImpl::close()
{
    if (m_currentPoint != m_startPoint)
    {
        m_commands.emplace_back(LineArgs{m_startPoint});
        m_currentPoint = m_startPoint;
    }
}

std::vector<std::variant<PathImpl::LineArgs, PathImpl::ArcArgs>> PathImpl::commands() const
{
    return m_commands;
}

Point PathImpl::startPoint() const
{
    return m_startPoint;
}
} // karin