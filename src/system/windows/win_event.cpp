#include "win_event.h"

#include <iostream>
#include <karin/system/event.h>

#include "win_converter.h"

#include <memory>
#include <utils/string.h>

namespace karin
{
std::optional<Event> translateWinEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        return KeyEvent(
            KeyEvent::Type::KeyPress_,
            winVirtualKeyToKeyCode(wParam),
            winScanCodeToKeyCode(lParam),
            getWinModifierState()
        );

    case WM_KEYUP:
        return KeyEvent(
            KeyEvent::Type::KeyRelease_,
            winVirtualKeyToKeyCode(wParam),
            winScanCodeToKeyCode(lParam),
            getWinModifierState()
        );

    case WM_CHAR:
    {
        std::wstring wsChar(1, static_cast<wchar_t>(wParam));
        return KeyTypeEvent(toString(wsChar));
    }

    case WM_LBUTTONDOWN:
        return MouseButtonEvent(
            MouseButtonEvent::Type::ButtonPress_,
            MouseButtonEvent::Button::Left,
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_LBUTTONUP:
        return MouseButtonEvent(
            MouseButtonEvent::Type::ButtonRelease_,
            MouseButtonEvent::Button::Left,
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_RBUTTONDOWN:
        return MouseButtonEvent(
            MouseButtonEvent::Type::ButtonPress_,
            MouseButtonEvent::Button::Right,
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_RBUTTONUP:
        return MouseButtonEvent(
            MouseButtonEvent::Type::ButtonRelease_,
            MouseButtonEvent::Button::Right,
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_MBUTTONDOWN:
        return MouseButtonEvent(
            MouseButtonEvent::Type::ButtonPress_,
            MouseButtonEvent::Button::Middle,
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_MBUTTONUP:
        return MouseButtonEvent(
            MouseButtonEvent::Type::ButtonRelease_,
            MouseButtonEvent::Button::Middle,
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_MOUSEMOVE:
        return MouseMoveEvent(
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_MOUSEWHEEL:
        return MouseWheelEvent(
            GET_WHEEL_DELTA_WPARAM(wParam),
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_CLOSE:
        return WindowEvent(WindowEvent::Type::Close);

    case WM_SIZE:
        switch (wParam)
        {
        case SIZE_MAXIMIZED:
            return WindowEvent(WindowEvent::Type::Maximize);
        case SIZE_MINIMIZED:
            return WindowEvent(WindowEvent::Type::Minimize);
        default:
            break;
        }
        return std::nullopt;

    case WM_KARIN_ACTION:
        return ActionEvent(
            static_cast<uint32_t>(wParam),
            *reinterpret_cast<std::shared_ptr<std::any>*>(lParam)
        );

    default:
        return std::nullopt;
    }
}
} // karin