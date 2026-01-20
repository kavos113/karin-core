#include "dwrite_font_loader.h"

#include <dwrite.h>
#include <wrl/client.h>
#include <iostream>
#include <string>

#include <d2d/d2d_context.h>
#include <utils/string.h>

#include "dwrite_font_face.h"
#include "dwrite_converter.h"

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