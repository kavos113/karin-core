#include <karin/common/rectangle.h>

namespace karin
{
bool Rectangle::operator==(const Rectangle &other) const
{
    return position == other.position && size == other.size;
}
} // karin