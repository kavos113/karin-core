#ifndef SRC_COMMON_UTILS_STRING_H
#define SRC_COMMON_UTILS_STRING_H

#ifdef KARIN_PLATFORM_WINDOWS
#include <windows.h>
#else
#include <clocale>
#include <cstdlib>
#endif

#include <string>
#include <vector>

inline std::string toString(const std::wstring& str)
{
    if (str.empty())
    {
        return "";
    }

#ifdef KARIN_PLATFORM_WINDOWS
    int newSize = WideCharToMultiByte(
        CP_UTF8,
        0,
        str.c_str(),
        static_cast<int>(str.size()),
        nullptr,
        0,
        nullptr,
        nullptr
    );
    if (newSize == 0)
    {
        return "";
    }
    std::vector<char> buffer(newSize);
    int result = WideCharToMultiByte(
        CP_UTF8,
        0,
        str.c_str(),
        static_cast<int>(str.size()),
        buffer.data(),
        newSize,
        nullptr,
        nullptr
    );
    if (result == 0)
    {
        return "";
    }

    return std::string(buffer.data(), newSize);
#else
    setlocale(LC_ALL, "");

    size_t newSize = std::wcstombs(nullptr, str.c_str(), 0);
    if (newSize == -1)
    {
        return "";
    }

    std::vector<char> buffer(newSize);
    size_t convertedSize = std::wcstombs(buffer.data(), str.c_str(), newSize);
    if (convertedSize == -1)
    {
        return "";
    }

    return {buffer.data()};
#endif
}

inline std::wstring toWString(const std::string& str)
{
    if (str.empty())
    {
        return L"";
    }

#ifdef KARIN_PLATFORM_WINDOWS
    int newSize = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        static_cast<int>(str.size()),
        nullptr,
        0
    );
    if (newSize == 0)
    {
        return L"";
    }
    std::vector<wchar_t> buffer(newSize);
    int result = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        static_cast<int>(str.size()),
        buffer.data(),
        newSize
    );
    if (result == 0)
    {
        return L"";
    }

    return std::wstring(buffer.data(), newSize);
#else
    setlocale(LC_ALL, "");

    size_t newSize = std::mbstowcs(nullptr, str.c_str(), 0);
    if (newSize == -1)
    {
        return L"";
    }

    std::vector<wchar_t> buffer(newSize);
    size_t convertedSize = std::mbstowcs(buffer.data(), str.c_str(), newSize);
    if (convertedSize == -1)
    {
        return L"";
    }
    return {buffer.data()};
#endif
}

#endif //SRC_COMMON_UTILS_STRING_H