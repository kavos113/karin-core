#include "win_event.h"

#include <karin/system/window_event.h>
#include <karin/system/key_event.h>
#include <karin/system/mouse_event.h>

#include <memory>

namespace karin
{
std::unique_ptr<Event> translateWinEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        return std::make_unique<CloseEvent>();

    case WM_SIZE:
        return std::make_unique<ResizeEvent>(
            static_cast<int>(LOWORD(lParam)),
            static_cast<int>(HIWORD(lParam))
        );

    case WM_PAINT:
        return std::make_unique<PaintEvent>();

    case WM_KEYDOWN:
        return std::make_unique<KeyEvent>(
            KeyEvent::Type::KeyPress,
            static_cast<int>(wParam)
        );

    case WM_KEYUP:
        return std::make_unique<KeyEvent>(
            KeyEvent::Type::KeyRelease,
            static_cast<int>(wParam)
        );

    case WM_MOUSEMOVE:
        return std::make_unique<MouseMoveEvent>(
            static_cast<int>(GET_X_LPARAM(lParam)),
            static_cast<int>(GET_Y_LPARAM(lParam))
        );

    case WM_LBUTTONDOWN:
        return std::make_unique<MouseEvent>(
            MouseEvent::Type::MouseButtonPress,
            static_cast<int>(GET_X_LPARAM(lParam)),
            static_cast<int>(GET_Y_LPARAM(lParam)),
            MouseEvent::Button::Left
        );
    }
}
} // karin