#ifndef KARIN_SYSTEM_EVENT_H
#define KARIN_SYSTEM_EVENT_H
#include <any>
#include <variant>
#include <string>

namespace karin
{

class KeyEvent;
class MouseMoveEvent;
class MouseButtonEvent;
class MouseWheelEvent;
class WindowEvent;
class WindowResizeEvent;
class ActionEvent;

using Event = std::variant<
    std::monostate,
    KeyEvent,
    MouseMoveEvent,
    MouseButtonEvent,
    MouseWheelEvent,
    WindowEvent,
    WindowResizeEvent,
    ActionEvent
>;

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
        // TODO: Add more buttons if needed
        Left,
        Right,
        Middle
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
        Paint
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

class KeyEvent
{
public:
    enum class KeyCode : uint8_t;
    enum class Modifier : uint8_t;
    enum class Type
    {
        KeyPress,
        KeyRelease,
        KeyType
    };

    KeyEvent(Type type, KeyCode keyCode, Modifier modifierState, const std::string& character)
        : m_type(type)
        , m_keyCode(keyCode)
        , m_modifierState(modifierState)
        , m_char(character)
    {
    }
private:
    Type m_type;
    KeyCode m_keyCode;
    Modifier m_modifierState;
    std::string m_char;

public:
    enum class Modifier : uint8_t
    {
        None    = 0x0,
        Shift   = 0x1,
        Control = 0x2,
        Alt     = 0x4,
        Super   = 0x8,
    };

    // based on USB HID Usage Page for Keyboard/Keypad (0x07)
    enum class KeyCode : uint8_t
    {
        ErrorRollOver = 0x01,
        POSTFail = 0x02,
        ErrorUndefined = 0x03,
        A = 0x04,
        B = 0x05,
        C = 0x06,
        D = 0x07,
        E = 0x08,
        F = 0x09,
        G = 0x0A,
        H = 0x0B,
        I = 0x0C,
        J = 0x0D,
        K = 0x0E,
        L = 0x0F,
        M = 0x10,
        N = 0x11,
        O = 0x12,
        P = 0x13,
        Q = 0x14,
        R = 0x15,
        S = 0x16,
        T = 0x17,
        U = 0x18,
        V = 0x19,
        W = 0x1A,
        X = 0x1B,
        Y = 0x1C,
        Z = 0x1D,
        Num_1 = 0x1E,
        Num_2 = 0x1F,
        Num_3 = 0x20,
        Num_4 = 0x21,
        Num_5 = 0x22,
        Num_6 = 0x23,
        Num_7 = 0x24,
        Num_8 = 0x25,
        Num_9 = 0x26,
        Num_0 = 0x27,
        Enter = 0x28,
        Escape = 0x29,
        Backspace = 0x2A,
        Tab = 0x2B,
        Space = 0x2C,
        Minus = 0x2D,
        Equal = 0x2E,
        LeftBracket = 0x2F,
        RightBracket = 0x30,
        Backslash = 0x31,
        NonUSHash = 0x32,
        Semicolon = 0x33,
        Apostrophe = 0x34,
        Grave = 0x35,
        Comma = 0x36,
        Period = 0x37,
        Slash = 0x38,
        CapsLock = 0x39,
        F1 = 0x3A,
        F2 = 0x3B,
        F3 = 0x3C,
        F4 = 0x3D,
        F5 = 0x3E,
        F6 = 0x3F,
        F7 = 0x40,
        F8 = 0x41,
        F9 = 0x42,
        F10 = 0x43,
        F11 = 0x44,
        F12 = 0x45,
        PrintScreen = 0x46,
        ScrollLock = 0x47,
        Pause = 0x48,
        Insert = 0x49,
        Home = 0x4A,
        PageUp = 0x4B,
        Delete = 0x4C,
        End = 0x4D,
        PageDown = 0x4E,
        RightArrow = 0x4F,
        LeftArrow = 0x50,
        DownArrow = 0x51,
        UpArrow = 0x52,
        NumLock = 0x53,
        KeypadSlash = 0x54,
        KeypadAsterisk = 0x55,
        KeypadMinus = 0x56,
        KeypadPlus = 0x57,
        KeypadEnter = 0x58,
        Keypad1 = 0x59,
        Keypad2 = 0x5A,
        Keypad3 = 0x5B,
        Keypad4 = 0x5C,
        Keypad5 = 0x5D,
        Keypad6 = 0x5E,
        Keypad7 = 0x5F,
        Keypad8 = 0x60,
        Keypad9 = 0x61,
        Keypad0 = 0x62,
        KeypadPeriod = 0x63,
        NonUSBackslash = 0x64,
        Application = 0x65,
        Power = 0x66,
        KeypadEqual = 0x67,
        F13 = 0x68,
        F14 = 0x69,
        F15 = 0x6A,
        F16 = 0x6B,
        F17 = 0x6C,
        F18 = 0x6D,
        F19 = 0x6E,
        F20 = 0x6F,
        F21 = 0x70,
        F22 = 0x71,
        F23 = 0x72,
        F24 = 0x73,
        Execute = 0x74,
        Help = 0x75,
        Menu = 0x76,
        Select = 0x77,
        Stop = 0x78,
        Again = 0x79,
        Undo = 0x7A,
        Cut = 0x7B,
        Copy = 0x7C,
        Paste = 0x7D,
        Find = 0x7E,
        Mute = 0x7F,
        VolumeUp = 0x80,
        VolumeDown = 0x81,
        LockingCapsLock = 0x82,
        LockingNumLock = 0x83,
        LockingScrollLock = 0x84,
        KeypadComma = 0x85,
        KeypadEqualSign = 0x86,
        International1 = 0x87,
        International2 = 0x88,
        International3 = 0x89,
        International4 = 0x8A,
        International5 = 0x8B,
        International6 = 0x8C,
        International7 = 0x8D,
        International8 = 0x8E,
        International9 = 0x8F,
        LANG1 = 0x90,
        LANG2 = 0x91,
        LANG3 = 0x92,
        LANG4 = 0x93,
        LANG5 = 0x94,
        LANG6 = 0x95,
        LANG7 = 0x96,
        LANG8 = 0x97,
        LANG9 = 0x98,
        AlternateErase = 0x99,
        SysReq = 0x9A,
        Cancel = 0x9B,
        Clear = 0x9C,
        Prior = 0x9D,
        Return = 0x9E,
        Separator = 0x9F,
        Out = 0xA0,
        Oper = 0xA1,
        ClearAgain = 0xA2,
        CrSel = 0xA3,
        ExSel = 0xA4,
        Keypad00 = 0xB0,
        Keypad000 = 0xB1,
        ThousandsSeparator = 0xB2,
        DecimalSeparator = 0xB3,
        CurrencyUnit = 0xB4,
        CurrencySubUnit = 0xB5,
        KeypadLeftParenthesis = 0xB6,
        KeypadRightParenthesis = 0xB7,
        KeypadLeftBrace = 0xB8,
        KeypadRightBrace = 0xB9,
        KeypadTab = 0xBA,
        KeypadBackspace = 0xBB,
        KeypadA = 0xBC,
        KeypadB = 0xBD,
        KeypadC = 0xBE,
        KeypadD = 0xBF,
        KeypadE = 0xC0,
        KeypadF = 0xC1,
        KeypadXOR = 0xC2,
        KeypadCaret = 0xC3,
        KeypadPercent = 0xC4,
        KeypadLessThan = 0xC5,
        KeypadGreaterThan = 0xC6,
        KeypadAmpersand = 0xC7,
        KeypadDoubleAmpersand = 0xC8,
        KeypadVerticalBar = 0xC9,
        KeypadDoubleVerticalBar = 0xCA,
        KeypadColon = 0xCB,
        KeypadHash = 0xCC,
        KeypadSpace = 0xCD,
        KeypadAtSign = 0xCE,
        KeypadExclamation = 0xCF,
        KeypadMemoryStore = 0xD0,
        KeypadMemoryRecall = 0xD1,
        KeypadMemoryClear = 0xD2,
        KeypadMemoryAdd = 0xD3,
        KeypadMemorySubtract = 0xD4,
        KeypadMemoryMultiply = 0xD5,
        KeypadMemoryDivide = 0xD6,
        KeypadPlusMinus = 0xD7,
        KeypadClear = 0xD8,
        KeypadClearEntry = 0xD9,
        KeypadBinary = 0xDA,
        KeypadOctal = 0xDB,
        KeypadDecimal = 0xDC,
        KeypadHexadecimal = 0xDD,
        LeftControl = 0xE0,
        LeftShift = 0xE1,
        LeftAlt = 0xE2,
        LeftGUI = 0xE3,
        RightControl = 0xE4,
        RightShift = 0xE5,
        RightAlt = 0xE6,
        RightGUI = 0xE7,
        Undefined = 0xFF
    };
};

constexpr KeyEvent::Modifier operator|(KeyEvent::Modifier a, KeyEvent::Modifier b)
{
    return static_cast<KeyEvent::Modifier>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

constexpr KeyEvent::Modifier operator&(KeyEvent::Modifier a, KeyEvent::Modifier b)
{
    return static_cast<KeyEvent::Modifier>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

constexpr KeyEvent::Modifier& operator|=(KeyEvent::Modifier& a, KeyEvent::Modifier b)
{
    a = a | b;
    return a;
}

} // karin

#endif //KARIN_SYSTEM_EVENT_H