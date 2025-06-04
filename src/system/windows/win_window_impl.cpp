#include "win_window_impl.h"

#include "win_window_class_registry.h"

namespace karin
{
std::once_flag WinWindowImpl::m_registerClassFlag;

WinWindowImpl::WinWindowImpl(
    const std::wstring &title,
    const int x,
    const int y,
    const int width,
    const int height
)
{
    std::call_once(m_registerClassFlag, &WinWindowImpl::registerClass);

    m_hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
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

void WinWindowImpl::registerClass()
{
    WNDCLASSEX wc = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = windowProc,
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

LRESULT WinWindowImpl::handleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_CLOSE:
            DestroyWindow(m_hwnd);
            break;

        default:
            break;
    }

    return DefWindowProc(m_hwnd, message, wParam, lParam);
}

LRESULT WinWindowImpl::windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WinWindowImpl* self = nullptr;

    if (message == WM_NCCREATE)
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
        self->handleMessage(message, wParam, lParam);
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
}

void WinWindowImpl::maximize()
{
    if (m_hwnd)
    {
        ShowWindow(m_hwnd, SW_MAXIMIZE);
    }
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

void* WinWindowImpl::handle() const
{
    return m_hwnd;
}
} // karin