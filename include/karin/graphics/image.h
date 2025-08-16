#ifndef KARIN_GRAPHICS_IMAGE_H
#define KARIN_GRAPHICS_IMAGE_H
#include <karin/common/geometry/size.h>

namespace karin
{
class Image
{
public:
    explicit Image(size_t hash, uint32_t width, uint32_t height)
        : m_hash(hash), m_width(width), m_height(height)
    {
    }

    size_t hash() const
    {
        return m_hash;
    }

    uint32_t width() const
    {
        return m_width;
    }

    uint32_t height() const
    {
        return m_height;
    }

private:
    size_t m_hash;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
};
} // karin

#endif //KARIN_GRAPHICS_IMAGE_H