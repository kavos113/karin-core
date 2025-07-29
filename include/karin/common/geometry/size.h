#ifndef KARIN_COMMON_SIZE_H
#define KARIN_COMMON_SIZE_H

#include <iostream>

namespace karin
{

struct Size
{
    float width;
    float height;

    bool operator==(const Size& other) const
    {
        return width == other.width && height == other.height;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Size& size)
{
    return os << "Size(" << size.width << ", " << size.height << ")";
}

} // karin

#endif //KARIN_COMMON_SIZE_H
