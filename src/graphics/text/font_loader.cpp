#include "font_loader.h"

#include <stdexcept>

namespace karin
{
FontLoader::FontLoader()
{
    FT_Error error = FT_Init_FreeType(&m_library);
    if (error)
    {
        throw std::runtime_error("Failed to initialize FreeType library");
    }
}

FontLoader::~FontLoader()
{
    FT_Done_FreeType(m_library);
}
} // karin
