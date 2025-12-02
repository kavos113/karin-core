#include "win_event.h"

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
            KeyEvent::Type::KeyPress,
            winVirtualKeyToKeyCode(wParam),
            getWinModifierState(),
            winKeyToChar(wParam, lParam)
        );

    case WM_KEYUP:
        return KeyEvent(
            KeyEvent::Type::KeyRelease,
            winVirtualKeyToKeyCode(wParam),
            getWinModifierState(),
            winKeyToChar(wParam, lParam)
        );

    case WM_CHAR:
    {
        std::wstring wsChar(1, static_cast<wchar_t>(wParam));
        return KeyEvent(
            KeyEvent::Type::KeyType,
            KeyEvent::KeyCode::Undefined,
            getWinModifierState(),
            toString(wsChar)
        );
    }

    case WM_LBUTTONDOWN:
        return MouseButtonEvent(
            MouseButtonEvent::Type::ButtonPress,
            MouseButtonEvent::Button::Left,
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_LBUTTONUP:
        return MouseButtonEvent(
            MouseButtonEvent::Type::ButtonRelease,
            MouseButtonEvent::Button::Left,
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_RBUTTONDOWN:
        return MouseButtonEvent(
            MouseButtonEvent::Type::ButtonPress,
            MouseButtonEvent::Button::Right,
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_RBUTTONUP:
        return MouseButtonEvent(
            MouseButtonEvent::Type::ButtonRelease,
            MouseButtonEvent::Button::Right,
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_MBUTTONDOWN:
        return MouseButtonEvent(
            MouseButtonEvent::Type::ButtonPress,
            MouseButtonEvent::Button::Middle,
            LOWORD(lParam),
            HIWORD(lParam)
        );

    case WM_MBUTTONUP:
        return MouseButtonEvent(
            MouseButtonEvent::Type::ButtonRelease,
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

    case WM_PAINT:
        return WindowEvent(WindowEvent::Type::Paint);

    case WM_SIZE:
        return WindowResizeEvent(
            LOWORD(lParam),
            HIWORD(lParam)
        );

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