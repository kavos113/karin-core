#include <karin/common/rectangle.h>

namespace karin
{
bool Rectangle::operator==(const Rectangle &other) const
{
    return pos == other.pos && size == other.size;
}
} // karin