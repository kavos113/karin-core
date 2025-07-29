#ifndef SRC_GRAPHICS_GRAPHICS_PATH_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_PATH_IMPL_H

#include <variant>
#include <vector>
#include <cstdint>

#include <karin/common/geometry/point.h>

namespace karin
{
class PathImpl
{
public:
    struct LineArgs
    {
        Point end;
    };

    struct ArcArgs
    {
        Point center;
        float radiusX;
        float radiusY;
        float startAngle;
        float endAngle;
        bool isSmallArc;
    };

    PathImpl();
    ~PathImpl() = default;

    void start(Point start);
    void lineTo(Point end);
    void arcTo(Point center, float radiusX, float radiusY, float startAngle, float endAngle, bool isSmallArc);
    void close();

    std::vector<std::variant<LineArgs, ArcArgs>> commands() const;
    Point startPoint() const;
    uint32_t id() const;

private:
    std::vector<std::variant<LineArgs, ArcArgs>> m_commands;
    Point m_startPoint;
    Point m_currentPoint;

    uint32_t m_id = 0;

    static uint32_t nextId;
};
} // karin

#endif //SRC_GRAPHICS_GRAPHICS_PATH_IMPL_H
