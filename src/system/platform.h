#ifndef SRC_SYSTEM_PLATFORM_H
#define SRC_SYSTEM_PLATFORM_H

#include <memory>
#include <string>

#include "application_impl.h"
#include "window_impl.h"

#ifdef KARIN_PLATFORM_WINDOWS
#include "windows/win_application_impl.h"
#include "windows/win_window_impl.h"
#endif

namespace karin
{

inline std::unique_ptr<IApplicationImpl> createApplicationImpl()
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<WinApplicationImpl>();
#endif

    return nullptr;
}

inline std::unique_ptr<IWindowImpl> createWindowImpl(
    const std::wstring &title,
    int x = 0,
    int y = 0,
    int width,
    int height
)
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<WinWindowImpl>(title, x, y, width, height);
#endif

    return nullptr;
}
}

#endif //SRC_SYSTEM_PLATFORM_H
