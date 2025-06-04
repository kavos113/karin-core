#ifndef KARIN_COMMON_SIZE_H
#define KARIN_COMMON_SIZE_H

namespace karin
{

struct Size
{
    float width;
    float height;

    bool operator==(const Size& other) const;
};

} // karin

#endif //KARIN_COMMON_SIZE_H
