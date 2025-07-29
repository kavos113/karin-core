#ifndef SRC_SYSTEM_PLATFORM_H
#define SRC_SYSTEM_PLATFORM_H

#include <memory>
#include <string>

#include "application_impl.h"
#include "window_impl.h"

#ifdef KARIN_PLATFORM_WINDOWS
#include "windows/win_application_impl.h"
#include "windows/win_window_impl.h"
#elifdef KARIN_PLATFORM_UNIX
#include "x11/x11_application_impl.h"
#include "x11/x11_window_impl.h"
#endif

namespace karin
{
inline std::unique_ptr<IApplicationImpl> createApplicationImpl()
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<WinApplicationImpl>();
#elifdef KARIN_PLATFORM_UNIX
    return std::make_unique<X11ApplicationImpl>();
#endif

    return nullptr;
}

inline std::unique_ptr<IWindowImpl> createWindowImpl(
    const std::wstring& title,
    int x,
    int y,
    int width,
    int height,
    IApplicationImpl* applicationImpl
)
{
#ifdef KARIN_PLATFORM_WINDOWS
    return std::make_unique<WinWindowImpl>(title, x, y, width, height);
#elifdef KARIN_PLATFORM_UNIX
    return std::make_unique<X11WindowImpl>(title, x, y, width, height, dynamic_cast<X11ApplicationImpl*>(applicationImpl));
#endif

    return nullptr;
}
}

#endif //SRC_SYSTEM_PLATFORM_H
