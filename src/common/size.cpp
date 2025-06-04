#include <karin/common/size.h>

namespace karin
{
bool Size::operator==(const Size &other) const
{
    return width == other.width && height == other.height;
}
} // karin