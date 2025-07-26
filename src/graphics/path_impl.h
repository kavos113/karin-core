#ifndef SRC_GRAPHICS_GRAPHICS_PATH_IMPL_H
#define SRC_GRAPHICS_GRAPHICS_PATH_IMPL_H

#include <variant>
#include <vector>

#include <karin/common/geometry/point.h>

namespace karin {

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
    };

    PathImpl() = default;
    ~PathImpl() = default;

    void start(Point start);
    void lineTo(Point end);
    void arcTo(Point center, float radiusX, float radiusY, float startAngle, float endAngle);
    void close();

    std::vector<std::variant<LineArgs, ArcArgs>> commands() const;
    Point startPoint() const;

private:
    std::vector<std::variant<LineArgs, ArcArgs>> m_commands;
    Point m_startPoint;
    Point m_currentPoint;
};

} // karin

#endif //SRC_GRAPHICS_GRAPHICS_PATH_IMPL_H
