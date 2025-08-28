#include "platform.h"

#include <karin/system/system_font.h>

namespace karin
{
std::vector<Font> getSystemFonts()
{
    return getPlatformSystemFonts();
}
}
