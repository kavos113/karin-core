#include "path_impl.h"

#include <cmath>

namespace karin
{
uint32_t PathImpl::nextId = 1;

PathImpl::PathImpl()
    : m_startPoint(0, 0),
      m_currentPoint(0, 0),
      m_id(nextId++)
{
}

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

void PathImpl::arcTo(Point center, float radiusX, float radiusY, float startAngle, float endAngle, bool isSmallArc)
{
    m_commands.emplace_back(ArcArgs{center, radiusX, radiusY, startAngle, endAngle, isSmallArc});
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

uint32_t PathImpl::id() const
{
    return m_id;
}
} // karin
