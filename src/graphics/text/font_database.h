#ifndef SRC_GRAPHICS_TEXT_FONT_DATABASE_H
#define SRC_GRAPHICS_TEXT_FONT_DATABASE_H

#include <memory>

#include "font_database_platform.h"

namespace karin
{
class FontDatabase
{
private:
    std::unique_ptr<IPlatformFontDatabase> m_platformDatabase;
};
} // karin

#endif //SRC_GRAPHICS_TEXT_FONT_DATABASE_H