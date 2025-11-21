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

bool WinApplicationImpl::pollEvent(Event &event)
{
    if (!m_eventQueue.empty())
    {
        event = std::move(m_eventQueue.front());
        m_eventQueue.pop();
        return true;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (msg.message == WM_QUIT)
        {
            m_isRunning = false;
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (!m_eventQueue.empty())
        {
            event = std::move(m_eventQueue.front());
            m_eventQueue.pop();
            return true;
        }
    }

    return false;
}

void WinApplicationImpl::shutdown()
{
    if (!m_isRunning)
        return;

    PostQuitMessage(0);
}
} // karin