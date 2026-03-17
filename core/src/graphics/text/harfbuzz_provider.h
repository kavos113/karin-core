#ifndef SRC_GRAPHICS_TEXT_HARFBUZZ_PROVIDER_H
#define SRC_GRAPHICS_TEXT_HARFBUZZ_PROVIDER_H

#include <hb.h>

namespace karin
{
class IHarfBuzzProvider
{
public:
    virtual ~IHarfBuzzProvider() = default;

    virtual hb_font_t* getHbFont() const = 0;
};
}

#endif //SRC_GRAPHICS_TEXT_HARFBUZZ_PROVIDER_H