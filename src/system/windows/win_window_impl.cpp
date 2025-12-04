#include "win_window_impl.h"

#include "win_event.h"
#include "win_window_class_registry.h"

namespace karin
{
WinWindowImpl::WinWindowImpl(
    const std::wstring& title,
    const int x,
    const int y,
    const int width,
    const int height,
    WinApplicationImpl* appImpl
)
    : m_appImpl(appImpl)
{
    m_hwnd = CreateWindowEx(
        0,
        WinApplicationImpl::CLASS_NAME,
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        x, y,
        width, height,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        this
    );
    if (!m_hwnd)
    {
        throw std::runtime_error("Failed to create window");
    }
}

LRESULT WinWindowImpl::handleMessage(UINT message, WPARAM wParam, LPARAM lParam) const
{
    std::optional<Event> event = translateWinEvent(message, wParam, lParam);
    if (event.has_value())
    {
        m_appImpl->pushEvent(*event);
    }

    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CLOSE:
        DestroyWindow(m_hwnd);
        return 0;

    case WM_PAINT:
        ValidateRect(m_hwnd, nullptr);
        if (m_onPaint)
        {
            m_onPaint();
        }
        return 0;

    case WM_SIZE:
        if (m_onResize)
        {
            Size newSize(LOWORD(lParam), HIWORD(lParam));
            m_onResize(newSize);
        }
        return 0;

    default:
        if (event.has_value())
        {
            return 0;
        }
    }

    return DefWindowProc(m_hwnd, message, wParam, lParam);
}

LRESULT WinWindowImpl::windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WinWindowImpl * self = nullptr;

    if (message == WM_CREATE)
    {
        auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        self = static_cast<WinWindowImpl*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));

        self->m_hwnd = hwnd;
    }
    else
    {
        self = reinterpret_cast<WinWindowImpl*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (self)
    {
        return self->handleMessage(message, wParam, lParam);
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

void WinWindowImpl::show()
{
    if (m_hwnd)
    {
        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);
    }
}

void WinWindowImpl::hide()
{
    if (m_hwnd)
    {
        ShowWindow(m_hwnd, SW_HIDE);
    }
}

void WinWindowImpl::minimize()
{
    if (m_hwnd)
    {
        ShowWindow(m_hwnd, SW_MINIMIZE);
    }

    m_appImpl->pushEvent(WindowEvent(WindowEvent::Type::Minimize));
}

void WinWindowImpl::maximize()
{
    if (m_hwnd)
    {
        ShowWindow(m_hwnd, SW_MAXIMIZE);
    }

    m_appImpl->pushEvent(WindowEvent(WindowEvent::Type::Maximize));
}

void WinWindowImpl::setPosition(int x, int y)
{
    SetWindowPos(
        m_hwnd,
        nullptr,
        x, y,
        0, 0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
    );
}

void WinWindowImpl::setSize(int width, int height)
{
    SetWindowPos(
        m_hwnd,
        nullptr,
        0, 0,
        width, height,
        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
    );
}

void WinWindowImpl::setRect(int x, int y, int width, int height)
{
    SetWindowPos(
        m_hwnd,
        nullptr,
        x, y,
        width, height,
        SWP_NOZORDER | SWP_NOACTIVATE
    );
}

Window::NativeHandle WinWindowImpl::handle() const
{
    return Window::NativeHandle{
        .hwnd = m_hwnd,
        .hinstance = GetModuleHandle(nullptr)
    };
}

void WinWindowImpl::setOnPaint(std::function<bool()> onPaint)
{
    m_onPaint = std::move(onPaint);
}

void WinWindowImpl::setOnResize(std::function<void(Size)> onResize)
{
    m_onResize = std::move(onResize);
}
} // karin