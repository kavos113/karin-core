#include "win_application_impl.h"

#include <windows.h>

#include "win_window_class_registry.h"
#include "win_window_impl.h"

namespace karin
{
WinApplicationImpl::WinApplicationImpl()
{
    WNDCLASSEX wc = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = WinWindowImpl::windowProc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = GetModuleHandle(nullptr),
        .hIcon = LoadIcon(nullptr, IDI_APPLICATION),
        .hCursor = LoadCursor(nullptr, IDC_ARROW),
        .hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
        .lpszMenuName = nullptr,
        .hIconSm = LoadIcon(nullptr, IDI_APPLICATION)
    };

    WinWindowClassRegistry::registerClass(wc, CLASS_NAME);
}

WinApplicationImpl::~WinApplicationImpl()
{
    WinWindowClassRegistry::unregisterClasses();
}

bool WinApplicationImpl::pollEvent(Event &event)
{
    if (!m_isRunning)
        m_isRunning = true;

    if (!m_eventQueue.empty())
    {
        event = m_eventQueue.front();
        m_eventQueue.pop();
        return true;
    }

    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
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
            event = m_eventQueue.front();
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