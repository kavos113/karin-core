#include "win_application_impl.h"

#include <windows.h>

#include "win_window_class_registry.h"

namespace karin
{
WinApplicationImpl::WinApplicationImpl()
{
}

WinApplicationImpl::~WinApplicationImpl()
{
}

void WinApplicationImpl::run()
{
    m_isRunning = true;

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    m_isRunning = false;

    WinWindowClassRegistry::unregisterClasses();
}

void WinApplicationImpl::shutdown()
{
    if (!m_isRunning)
        return;

    PostQuitMessage(0);
}
} // karin
