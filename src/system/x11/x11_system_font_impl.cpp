#include "x11_system_font_impl.h"

#include <fontconfig/fontconfig.h>

#include <stdexcept>
#include <vector>
#include <fstream>

namespace
{
using namespace karin;

Font::Style toFontStyle(int slant)
{
    switch (slant)
    {
    case FC_SLANT_ITALIC:
        return Font::Style::ITALIC;
    case FC_SLANT_OBLIQUE:
        return Font::Style::OBLIQUE;
    default:
        return Font::Style::NORMAL;
    }
}

Font::Stretch toFontStretch(int width)
{
    if (width <= FC_WIDTH_ULTRACONDENSED)
        return Font::Stretch::ULTRA_CONDENSED;
    else if (width <= FC_WIDTH_EXTRACONDENSED)
        return Font::Stretch::EXTRA_CONDENSED;
    else if (width <= FC_WIDTH_CONDENSED)
        return Font::Stretch::CONDENSED;
    else if (width <= FC_WIDTH_SEMICONDENSED)
        return Font::Stretch::SEMI_CONDENSED;
    else if (width == FC_WIDTH_NORMAL)
        return Font::Stretch::NORMAL;
    else if (width <= FC_WIDTH_SEMIEXPANDED)
        return Font::Stretch::SEMI_EXPANDED;
    else if (width <= FC_WIDTH_EXPANDED)
        return Font::Stretch::EXPANDED;
    else if (width <= FC_WIDTH_EXTRAEXPANDED)
        return Font::Stretch::EXTRA_EXPANDED;
    else
        return Font::Stretch::ULTRA_EXPANDED;
}

Font::Weight toFontWeight(int weight)
{
    if (weight <= FC_WEIGHT_THIN)
        return Font::Weight::THIN;
    else if (weight <= FC_WEIGHT_EXTRALIGHT)
        return Font::Weight::EXTRA_LIGHT;
    else if (weight <= FC_WEIGHT_LIGHT)
        return Font::Weight::LIGHT;
    else if (weight <= FC_WEIGHT_DEMILIGHT)
        return Font::Weight::SEMI_LIGHT;
    else if (weight <= FC_WEIGHT_REGULAR)
        return Font::Weight::NORMAL;
    else if (weight <= FC_WEIGHT_MEDIUM)
        return Font::Weight::MEDIUM;
    else if (weight <= FC_WEIGHT_DEMIBOLD)
        return Font::Weight::SEMI_BOLD;
    else if (weight <= FC_WEIGHT_BOLD)
        return Font::Weight::BOLD;
    else if (weight <= FC_WEIGHT_EXTRABOLD)
        return Font::Weight::EXTRA_BOLD;
    else if (weight <= FC_WEIGHT_BLACK)
        return Font::Weight::BLACK;
    else
        return Font::Weight::EXTRA_BLACK;
}

}

namespace karin
{
X11SystemFontImpl::X11SystemFontImpl()
{
    if (!FcInit())
    {
        throw std::runtime_error("Failed to initialize Fontconfig.");
    }
}

X11SystemFontImpl::~X11SystemFontImpl()
{
    FcFini();
}

std::vector<Font> X11SystemFontImpl::getSystemFonts()
{
    std::vector<Font> fonts;

    FcPattern* pat = FcPatternCreate();
    FcObjectSet* os = FcObjectSetBuild(
        FC_FAMILY,
        FC_STYLE,
        FC_SLANT,
        FC_WEIGHT,
        FC_WIDTH,
        nullptr
    );
    FcFontSet* fs = FcFontList(nullptr, pat, os);

    for (int i = 0; i < fs->nfont; ++i)
    {
        FcPattern* fontPat = fs->fonts[i];

        char* family = nullptr;
        if (FcPatternGetString(fontPat, FC_FAMILY, 0, reinterpret_cast<FcChar8**>(&family)) != FcResultMatch)
        {
            continue;
        }

        char* style = nullptr;
        if (FcPatternGetString(fontPat, FC_STYLE, 0, reinterpret_cast<FcChar8**>(&style)) != FcResultMatch)
        {
            style = const_cast<char*>("Regular");
        }

        int slant = FC_SLANT_ROMAN;
        FcPatternGetInteger(fontPat, FC_SLANT, 0, &slant);

        int weight = FC_WEIGHT_NORMAL;
        FcPatternGetInteger(fontPat, FC_WEIGHT, 0, &weight);

        int width = FC_WIDTH_NORMAL;
        FcPatternGetInteger(fontPat, FC_WIDTH, 0, &width);

        fonts.push_back(
            Font{
                .family = std::string(family),
                .style = toFontStyle(slant),
                .stretch = toFontStretch(width),
                .weight = toFontWeight(weight),
            }
        );
    }

    FcFontSetDestroy(fs);
    FcObjectSetDestroy(os);
    FcPatternDestroy(pat);

    return fonts;
}

std::vector<std::byte> X11SystemFontImpl::getFontData(const Font& font)
{
    FcPattern* pat = FcPatternCreate();
    FcObjectSet* os = FcObjectSetBuild(
        FC_FAMILY,
        FC_STYLE,
        FC_SLANT,
        FC_WEIGHT,
        FC_WIDTH,
        nullptr
    );
    FcFontSet* fs = FcFontList(nullptr, pat, os);

    for (int i = 0; i < fs->nfont; ++i)
    {
        FcPattern* fontPat = fs->fonts[i];

        char* family = nullptr;
        if (FcPatternGetString(fontPat, FC_FAMILY, 0, reinterpret_cast<FcChar8**>(&family)) != FcResultMatch)
        {
            continue;
        }

        char* style = nullptr;
        if (FcPatternGetString(fontPat, FC_STYLE, 0, reinterpret_cast<FcChar8**>(&style)) != FcResultMatch)
        {
            style = const_cast<char*>("Regular");
        }

        int slant = FC_SLANT_ROMAN;
        FcPatternGetInteger(fontPat, FC_SLANT, 0, &slant);

        int weight = FC_WEIGHT_NORMAL;
        FcPatternGetInteger(fontPat, FC_WEIGHT, 0, &weight);

        int width = FC_WIDTH_NORMAL;
        FcPatternGetInteger(fontPat, FC_WIDTH, 0, &width);

        if (font.family == family &&
            font.style == toFontStyle(slant) &&
            font.weight == toFontWeight(weight) &&
            font.stretch == toFontStretch(width))
        {
            char* filePath = nullptr;
            if (FcPatternGetString(fontPat, FC_FILE, 0, reinterpret_cast<FcChar8**>(&filePath)) != FcResultMatch)
            {
                continue;
            }

            std::ifstream file(filePath, std::ios::binary | std::ios::ate);
            if (!file)
            {
                continue;
            }

            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<std::byte> buffer(size);
            if (file.read(reinterpret_cast<char*>(buffer.data()), size))
            {
                FcFontSetDestroy(fs);
                FcObjectSetDestroy(os);
                FcPatternDestroy(pat);
                return buffer;
            }
        }
    }

    FcFontSetDestroy(fs);
    FcObjectSetDestroy(os);
    FcPatternDestroy(pat);

    return {};
}
} // karin