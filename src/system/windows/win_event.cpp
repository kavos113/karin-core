#include "win_event.h"

#include <karin/system/event.h>

#include "win_converter.h"

#include <memory>
#include <utils/string.h>

namespace karin
{
Event translateWinEvent(UINT message, WPARAM wParam, LPARAM lParam)
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
}
} // karin