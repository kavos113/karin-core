#ifndef KARIN_SYSTEM_KEY_EVENT_H
#define KARIN_SYSTEM_KEY_EVENT_H

#include "event.h"

namespace karin
{
class KeyEvent : public Event
{
public:
    enum class KeyCode
    {
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        MINUS, EQUALS, LEFT_BRACKET, RIGHT_BRACKET, BACKSLASH, SEMICOLON, APOSTROPHE, GRAVE, COMMA, PERIOD, SLASH, AT,
        NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
        ENTER, ESCAPE, BACKSPACE, TAB, SPACE,
        LEFT_SHIFT, RIGHT_SHIFT, LEFT_CTRL, RIGHT_CTRL, LEFT_ALT, RIGHT_ALT,
        LEFT_ARROW, RIGHT_ARROW, UP_ARROW, DOWN_ARROW,
        INSERT, DELETE, HOME, END, PAGE_UP, PAGE_DOWN,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        NUMPAD_0, NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4, NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9,
        NUMPAD_ADD, NUMPAD_SUBTRACT, NUMPAD_MULTIPLY, NUMPAD_DIVIDE, NUMPAD_DECIMAL,
        NUMPAD_ENTER,
        NUM_LOCK, CAPS_LOCK, SCROLL_LOCK, PRINT_SCREEN, PAUSE,
        WINDOWS, KANA, KANJI, CONVERT, NON_CONVERT,

        UNKNOWN
    };

    KeyEvent(Type type, KeyCode keyCode)
        : Event(type)
        , m_keyCode(keyCode)
    {
    }
private:
    KeyCode m_keyCode;
};
} // karin

#endif //KARIN_SYSTEM_KEY_EVENT_H