#ifndef KARIN_COMMON_SIZE_H
#define KARIN_COMMON_SIZE_H

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

} // karin

#endif //KARIN_COMMON_SIZE_H
