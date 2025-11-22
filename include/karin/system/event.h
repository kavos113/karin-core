#ifndef KARIN_SYSTEM_EVENT_H
#define KARIN_SYSTEM_EVENT_H
#include <any>
#include <variant>

namespace karin
{
class KeyEvent
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

    enum class Type
    {
        KeyPress,
        KeyRelease,
    };

    KeyEvent(Type type, KeyCode keyCode)
        : m_type(type)
        , m_keyCode(keyCode)
    {
    }
private:
    Type m_type;
    KeyCode m_keyCode;
};

class MouseMoveEvent
{
public:
    MouseMoveEvent(int x, int y)
        : m_x(x)
        , m_y(y)
    {
    }

private:
    int m_x;
    int m_y;

};

class MouseButtonEvent
{
public:
    enum class Button
    {
        LEFT,
        RIGHT,
        MIDDLE
    };

    enum class Type
    {
        ButtonPress,
        ButtonRelease
    };

    MouseButtonEvent(Type type, Button button, int x, int y)
        : m_type(type)
        , m_button(button)
        , m_x(x)
        , m_y(y)
    {
    }

private:
    Type m_type;
    Button m_button;
    int m_x;
    int m_y;
};

class MouseWheelEvent
{
public:
    MouseWheelEvent(int delta, int x, int y)
        : m_delta(delta)
        , m_x(x)
        , m_y(y)
    {
    }

private:
    int m_delta;
    int m_x;
    int m_y;
};

class WindowEvent
{
public:
    enum class Type
    {
        Close,
        Maximize,
        Minimize,
        Resize,
    };

    WindowEvent(Type type)
        : m_type(type)
    {
    }

private:
    Type m_type;
};

class WindowResizeEvent
{
public:
    WindowResizeEvent(int width, int height)
        : m_width(width)
        , m_height(height)
    {
    }

private:
    int m_width;
    int m_height;
};

class ActionEvent
{
public:
    ActionEvent(uint32_t actionId, std::any data = {})
        : m_actionId(actionId)
        , m_data(std::move(data))
    {
    }

private:
    uint32_t m_actionId;
    std::any m_data;
};

using Event = std::variant<
    KeyEvent,
    MouseMoveEvent,
    MouseButtonEvent,
    MouseWheelEvent,
    WindowEvent,
    WindowResizeEvent,
    ActionEvent
>;

} // karin

#endif //KARIN_SYSTEM_EVENT_H