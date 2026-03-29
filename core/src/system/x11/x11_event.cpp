#include "x11_event.h"

#include "x11_converter.h"

namespace
{
constexpr float SCROLL_DELTA = 1.0f;
}

namespace karin
{
std::optional<Event> translateX11Event(XEvent* event)
{
    switch (event->type)
    {
    case ButtonPress:
    {
        int x = event->xbutton.x;
        int y = event->xbutton.y;
        switch (event->xbutton.button)
        {
        case Button1:
            return MouseButtonEvent(
                MouseButtonEvent::Type::ButtonPress_,
                MouseButtonEvent::Button::Left,
                x, y
            );
        case Button2:
            return MouseButtonEvent(
                MouseButtonEvent::Type::ButtonPress_,
                MouseButtonEvent::Button::Middle,
                x, y
            );
        case Button3:
            return MouseButtonEvent(
                MouseButtonEvent::Type::ButtonPress_,
                MouseButtonEvent::Button::Right,
                x, y
            );
        case Button4:
            return MouseWheelEvent(
                SCROLL_DELTA,
                x, y
            );
        case Button5:
            return MouseWheelEvent(
                -SCROLL_DELTA,
                x, y
            );
        default:
            break;
        }
        break;
    }

    case ButtonRelease:
    {
        int x = event->xbutton.x;
        int y = event->xbutton.y;
        switch (event->xbutton.button)
        {
        case Button1:
            return MouseButtonEvent(
                MouseButtonEvent::Type::ButtonRelease_,
                MouseButtonEvent::Button::Left,
                x, y
            );
        case Button2:
            return MouseButtonEvent(
                MouseButtonEvent::Type::ButtonRelease_,
                MouseButtonEvent::Button::Middle,
                x, y
            );
        case Button3:
            return MouseButtonEvent(
                MouseButtonEvent::Type::ButtonRelease_,
                MouseButtonEvent::Button::Right,
                x, y
            );
        default:
            break;
        }
        break;
    }

    case KeyPress:
    {
        KeySym keysym = XLookupKeysym(&event->xkey, 0);
        return KeyEvent(
            KeyEvent::Type::KeyPress_,
            x11ConvertKeyCode(event->xkey.keycode),
            x11ConvertKeySym(keysym),
            x11ConvertModifier(event->xkey.state)
        );
    }

    case KeyRelease:
    {
        KeySym keysym = XLookupKeysym(&event->xkey, 0);
        return KeyEvent(
            KeyEvent::Type::KeyRelease_,
            x11ConvertKeyCode(event->xkey.keycode),
            x11ConvertKeySym(keysym),
            x11ConvertModifier(event->xkey.state)
        );
    }

    default:
        return std::nullopt;
    }

    return std::nullopt;
}
}
