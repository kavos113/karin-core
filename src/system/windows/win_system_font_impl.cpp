#include "win_system_font_impl.h"

#include <utils/string.h>
#include <d2d/dwrite_converter.h>

#include <dwrite.h>
#include <wrl/client.h>

#include <iostream>
#include <stdexcept>

namespace karin
{
WinSystemFontImpl::WinSystemFontImpl()
{
    HRESULT hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(m_factory.GetAddressOf())
    );
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create DirectWrite factory.");
    }
}

std::vector<Font> WinSystemFontImpl::getSystemFonts()
{
    Microsoft::WRL::ComPtr<IDWriteFontCollection> fontCollection;
    HRESULT hr = m_factory->GetSystemFontCollection(&fontCollection);
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

std::vector<std::byte> WinSystemFontImpl::getFontData(const Font& font)
{
    Microsoft::WRL::ComPtr<IDWriteFontCollection> fontCollection;
    HRESULT hr = m_factory->GetSystemFontCollection(&fontCollection);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get system font collection." << std::endl;
        return {};
    }

    std::wstring wFamily = toWString(font.family);
    UINT32 index = 0;
    BOOL exists = FALSE;
    hr = fontCollection->FindFamilyName(wFamily.c_str(), &index, &exists);
    if (FAILED(hr) || !exists)
    {
        std::cerr << "Font family not found: " << font.family << std::endl;
        return {};
    }

    Microsoft::WRL::ComPtr<IDWriteFontFamily> fontFamily;
    hr = fontCollection->GetFontFamily(index, &fontFamily);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get font family." << std::endl;
        return {};
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
        std::cerr << "Failed to find matching font." << std::endl;
        return {};
    }

    Microsoft::WRL::ComPtr<IDWriteFontFace> fontFace;
    hr = targetFont->CreateFontFace(&fontFace);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create font face." << std::endl;
        return {};
    }

    Microsoft::WRL::ComPtr<IDWriteFontFile> fontFile;
    UINT32 fileCount = 0;
    hr = fontFace->GetFiles(&fileCount, nullptr);
    if (FAILED(hr) || fileCount == 0)
    {
        std::cerr << "No font files found." << std::endl;
        return {};
    }
    fileCount = 1;
    hr = fontFace->GetFiles(&fileCount, &fontFile);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get font file." << std::endl;
        return {};
    }

    const void* refKey = nullptr;
    UINT32 refKeySize = 0;
    hr = fontFile->GetReferenceKey(&refKey, &refKeySize);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get reference key." << std::endl;
        return {};
    }

    Microsoft::WRL::ComPtr<IDWriteFontFileLoader> fileLoader;
    hr = fontFile->GetLoader(&fileLoader);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get font file loader." << std::endl;
        return {};
    }

    Microsoft::WRL::ComPtr<IDWriteFontFileStream> fileStream;
    hr = fileLoader->CreateStreamFromKey(refKey, refKeySize, &fileStream);
    if (FAILED(hr))
    {
        std::cerr << "Failed to create font file stream." << std::endl;
        return {};
    }

    UINT64 fileSize = 0;
    hr = fileStream->GetFileSize(&fileSize);
    if (FAILED(hr))
    {
        std::cerr << "Failed to get font file size." << std::endl;
        return {};
    }

    std::vector<std::byte> fontData(fileSize);
    const void* fragmentStart = nullptr;
    void* context = nullptr;
    hr = fileStream->ReadFileFragment(&fragmentStart, 0, fileSize, &context);
    if (FAILED(hr))
    {
        std::cerr << "Failed to read font file fragment." << std::endl;
        return {};
    }

    memcpy(fontData.data(), fragmentStart, fileSize);

    fileStream->ReleaseFileFragment(context);

    return fontData;
}
}
