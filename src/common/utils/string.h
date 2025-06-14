#ifndef SRC_COMMON_UTILS_STRING_H
#define SRC_COMMON_UTILS_STRING_H
#include <string>
#include <vector>

inline std::string toString(std::wstring str) {
    const wchar_t* wcs = str.c_str();
    size_t size = wcstombs(nullptr, wcs, 0);
    if (size == static_cast<size_t>(-1)) {
        return "";
    }

    std::vector<char> buffer(size + 1);
    wcstombs(buffer.data(), wcs, size + 1);
    return std::string(buffer.data());
}

#endif //SRC_COMMON_UTILS_STRING_H
