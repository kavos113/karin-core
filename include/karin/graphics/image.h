#ifndef KARIN_GRAPHICS_IMAGE_H
#define KARIN_GRAPHICS_IMAGE_H
#include <karin/common/geometry/size.h>
#include <vector>

namespace karin
{
class Image
{
private:
    friend class Renderer;

    explicit Image(size_t hash, Size size)
        : m_hash(hash), m_size(size)
    {
    }

    size_t m_hash;
    Size m_size;
};
} // karin

#endif //KARIN_GRAPHICS_IMAGE_H
