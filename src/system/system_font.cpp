#include "platform.h"

#include <karin/system/system_font.h>

namespace karin
{
std::vector<SystemFontInfo> getSystemFonts()
{
    return getPlatformSystemFonts();
}
}