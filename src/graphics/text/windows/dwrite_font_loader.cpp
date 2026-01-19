#include "dwrite_font_loader.h"

#include <dwrite.h>
#include <wrl/client.h>
#include <iostream>
#include <string>

#include <d2d/d2d_context.h>
#include <utils/string.h>

#include "dwrite_font_face.h"

namespace
{
using namespace karin;

DWRITE_FONT_WEIGHT toDWriteFontWeight(Font::Weight weight)
{
    switch (weight)
    {
    case Font::Weight::THIN:
        return DWRITE_FONT_WEIGHT_THIN;
    case Font::Weight::EXTRA_LIGHT:
        return DWRITE_FONT_WEIGHT_EXTRA_LIGHT;
    case Font::Weight::LIGHT:
        return DWRITE_FONT_WEIGHT_LIGHT;
    case Font::Weight::SEMI_LIGHT:
        return DWRITE_FONT_WEIGHT_SEMI_LIGHT;
    case Font::Weight::NORMAL:
        return DWRITE_FONT_WEIGHT_NORMAL;
    case Font::Weight::MEDIUM:
        return DWRITE_FONT_WEIGHT_MEDIUM;
    case Font::Weight::SEMI_BOLD:
        return DWRITE_FONT_WEIGHT_SEMI_BOLD;
    case Font::Weight::BOLD:
        return DWRITE_FONT_WEIGHT_BOLD;
    case Font::Weight::EXTRA_BOLD:
        return DWRITE_FONT_WEIGHT_EXTRA_BOLD;
    case Font::Weight::BLACK:
        return DWRITE_FONT_WEIGHT_BLACK;
    case Font::Weight::EXTRA_BLACK:
        return DWRITE_FONT_WEIGHT_EXTRA_BLACK;
    default:
        throw std::invalid_argument("Unknown font weight");
    }
}

DWRITE_FONT_STYLE toDWriteFontStyle(Font::Style style)
{
    switch (style)
    {
    case Font::Style::NORMAL:
        return DWRITE_FONT_STYLE_NORMAL;
    case Font::Style::ITALIC:
        return DWRITE_FONT_STYLE_ITALIC;
    case Font::Style::OBLIQUE:
        return DWRITE_FONT_STYLE_OBLIQUE;
    default:
        throw std::invalid_argument("Unknown font style");
    }
}

DWRITE_FONT_STRETCH toDWriteFontStretch(Font::Stretch stretch)
{
    switch (stretch)
    {
    case Font::Stretch::ULTRA_CONDENSED:
        return DWRITE_FONT_STRETCH_ULTRA_CONDENSED;
    case Font::Stretch::EXTRA_CONDENSED:
        return DWRITE_FONT_STRETCH_EXTRA_CONDENSED;
    case Font::Stretch::CONDENSED:
        return DWRITE_FONT_STRETCH_CONDENSED;
    case Font::Stretch::SEMI_CONDENSED:
        return DWRITE_FONT_STRETCH_SEMI_CONDENSED;
    case Font::Stretch::NORMAL:
        return DWRITE_FONT_STRETCH_NORMAL;
    case Font::Stretch::SEMI_EXPANDED:
        return DWRITE_FONT_STRETCH_SEMI_EXPANDED;
    case Font::Stretch::EXPANDED:
        return DWRITE_FONT_STRETCH_EXPANDED;
    case Font::Stretch::EXTRA_EXPANDED:
        return DWRITE_FONT_STRETCH_EXTRA_EXPANDED;
    case Font::Stretch::ULTRA_EXPANDED:
        return DWRITE_FONT_STRETCH_ULTRA_EXPANDED;
    default:
        throw std::invalid_argument("Unknown font stretch");
    }
}

Font::Weight fromDWriteFontWeight(DWRITE_FONT_WEIGHT weight)
{
    switch (weight)
    {
    case DWRITE_FONT_WEIGHT_THIN:
        return Font::Weight::THIN;
    case DWRITE_FONT_WEIGHT_EXTRA_LIGHT:
        return Font::Weight::EXTRA_LIGHT;
    case DWRITE_FONT_WEIGHT_LIGHT:
        return Font::Weight::LIGHT;
    case DWRITE_FONT_WEIGHT_SEMI_LIGHT:
        return Font::Weight::SEMI_LIGHT;
    case DWRITE_FONT_WEIGHT_NORMAL:
        return Font::Weight::NORMAL;
    case DWRITE_FONT_WEIGHT_MEDIUM:
        return Font::Weight::MEDIUM;
    case DWRITE_FONT_WEIGHT_SEMI_BOLD:
        return Font::Weight::SEMI_BOLD;
    case DWRITE_FONT_WEIGHT_BOLD:
        return Font::Weight::BOLD;
    case DWRITE_FONT_WEIGHT_EXTRA_BOLD:
        return Font::Weight::EXTRA_BOLD;
    case DWRITE_FONT_WEIGHT_BLACK:
        return Font::Weight::BLACK;
    case DWRITE_FONT_WEIGHT_EXTRA_BLACK:
        return Font::Weight::EXTRA_BLACK;
    default:
        throw std::invalid_argument("Unknown DWRITE font weight");
    }
}

Font::Style fromDWriteFontStyle(DWRITE_FONT_STYLE style)
{
    switch (style)
    {
    case DWRITE_FONT_STYLE_NORMAL:
        return Font::Style::NORMAL;
    case DWRITE_FONT_STYLE_ITALIC:
        return Font::Style::ITALIC;
    case DWRITE_FONT_STYLE_OBLIQUE:
        return Font::Style::OBLIQUE;
    default:
        throw std::invalid_argument("Unknown DWRITE font style");
    }
}

Font::Stretch fromDWriteFontStretch(DWRITE_FONT_STRETCH stretch)
{
    switch (stretch)
    {
    case DWRITE_FONT_STRETCH_ULTRA_CONDENSED:
        return Font::Stretch::ULTRA_CONDENSED;
    case DWRITE_FONT_STRETCH_EXTRA_CONDENSED:
        return Font::Stretch::EXTRA_CONDENSED;
    case DWRITE_FONT_STRETCH_CONDENSED:
        return Font::Stretch::CONDENSED;
    case DWRITE_FONT_STRETCH_SEMI_CONDENSED:
        return Font::Stretch::SEMI_CONDENSED;
    case DWRITE_FONT_STRETCH_NORMAL:
        return Font::Stretch::NORMAL;
    case DWRITE_FONT_STRETCH_SEMI_EXPANDED:
        return Font::Stretch::SEMI_EXPANDED;
    case DWRITE_FONT_STRETCH_EXPANDED:
        return Font::Stretch::EXPANDED;
    case DWRITE_FONT_STRETCH_EXTRA_EXPANDED:
        return Font::Stretch::EXTRA_EXPANDED;
    case DWRITE_FONT_STRETCH_ULTRA_EXPANDED:
        return Font::Stretch::ULTRA_EXPANDED;
    default:
        throw std::invalid_argument("Unknown DWRITE font stretch");
    }
}
}

namespace karin
{
DwriteFontLoader::~DwriteFontLoader() = default;

std::unique_ptr<IFontFace> DwriteFontLoader::loadFont(const Font& font)
{
    Microsoft::WRL::ComPtr<IDWriteFontCollection> fontCollection;
    HRESULT hr = D2DContext::instance().dwriteFactory()->GetSystemFontCollection(&fontCollection);
    if (FAILED(hr))
    {
        std::cerr << "failed to get system font collection" << std::endl;
        return nullptr;
    }

    std::wstring wFamily = toWString(font.family);
    UINT32 index = 0;
    BOOL exists = FALSE;
    hr = fontCollection->FindFamilyName(wFamily.c_str(), &index, &exists);
    if (FAILED(hr) || !exists)
    {
        std::cerr << "font family not found: " << font.family << std::endl;
        return nullptr;
    }

    Microsoft::WRL::ComPtr<IDWriteFontFamily> fontFamily;
    hr = fontCollection->GetFontFamily(index, &fontFamily);
    if (FAILED(hr))
    {
        std::cerr << "failed to get font family" << std::endl;
        return nullptr;
    }

    Microsoft::WRL::ComPtr<IDWriteFont> targetFont;
    hr = fontFamily->GetFirstMatchingFont(
        toDWriteFontWeight(font.weight),
        toDWriteFontStretch(font.stretch),
        toDWriteFontStyle(font.style),
        &targetFont
    );
    if (FAILED(hr))
    {
        std::cerr << "failed to find matching font" << std::endl;
        return nullptr;
    }

    Microsoft::WRL::ComPtr<IDWriteFontFace> face;
    hr = targetFont->CreateFontFace(&face);
    if (FAILED(hr))
    {
        std::cerr << "failed to create font face" << std::endl;
        return nullptr;
    }

    return std::make_unique<DwriteFontFace>(face);
}

std::vector<Font> DwriteFontLoader::getFontLists()
{
    Microsoft::WRL::ComPtr<IDWriteFontCollection> fontCollection;
    HRESULT hr = D2DContext::instance().dwriteFactory()->GetSystemFontCollection(&fontCollection);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get system font collection." << std::endl;
        return {};
    }

    UINT32 fontCount = fontCollection->GetFontFamilyCount();
    std::wcout << L"Number of font families: " << fontCount << std::endl;

    WCHAR localeName[LOCALE_NAME_MAX_LENGTH];
    int localeLength = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);
    if (localeLength == 0)
    {
        std::wcerr << L"Failed to get user default locale name." << std::endl;
        wcscpy_s(localeName, L"en-US");
    }

    std::vector<Font> fonts;

    for (UINT32 i = 0; i < fontCount; ++i)
    {
        Microsoft::WRL::ComPtr<IDWriteFontFamily> fontFamily;
        hr = fontCollection->GetFontFamily(i, &fontFamily);
        if (FAILED(hr))
        {
            std::cerr << "Failed to get font family." << std::endl;
            continue;
        }

        Microsoft::WRL::ComPtr<IDWriteLocalizedStrings> familyNames;
        hr = fontFamily->GetFamilyNames(&familyNames);
        if (FAILED(hr))
        {
            std::cerr << "Failed to get family names." << std::endl;
            continue;
        }
        UINT32 index = 0;
        BOOL exists = FALSE;
        hr = familyNames->FindLocaleName(localeName, &index, &exists);
        if (FAILED(hr))
        {
            std::cerr << "Failed to find locale name." << std::endl;
            continue;
        }
        if (!exists)
        {
            hr = familyNames->FindLocaleName(L"en-US", &index, &exists);
            if (FAILED(hr) || !exists)
            {
                index = 0; // Fallback to the first entry
            }
        }
        UINT32 length = 0;
        hr = familyNames->GetStringLength(index, &length);
        if (FAILED(hr))
        {
            std::cerr << "Failed to get string length." << std::endl;
            continue;
        }
        std::vector<WCHAR> nameBuffer(length + 1);
        hr = familyNames->GetString(index, nameBuffer.data(), length + 1);
        if (FAILED(hr))
        {
            std::cerr << "Failed to get family name string." << std::endl;
            continue;
        }
        std::wstring familyName(nameBuffer.data());

        UINT32 numFonts = fontFamily->GetFontCount();
        for (UINT32 j = 0; j < numFonts; ++j)
        {
            Microsoft::WRL::ComPtr<IDWriteFont> font;
            hr = fontFamily->GetFont(j, &font);
            if (FAILED(hr))
            {
                std::cerr << "Failed to get font." << std::endl;
                continue;
            }

            DWRITE_FONT_WEIGHT weight = font->GetWeight();
            DWRITE_FONT_STYLE style = font->GetStyle();
            DWRITE_FONT_STRETCH stretch = font->GetStretch();

            fonts.push_back(
                Font{
                    .family = toString(familyName),
                    .style = fromDWriteFontStyle(style),
                    .stretch = fromDWriteFontStretch(stretch),
                    .weight = fromDWriteFontWeight(weight),
                }
            );
        }
    }

    return fonts;
}
} // karin