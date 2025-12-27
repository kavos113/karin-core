#include "x11_converter.h"

#include <X11/keysym.h>

namespace
{
using namespace karin;
using enum KeyEvent::KeyCode;
KeyEvent::KeyCode x11ScanCodeToKeyCode[256] = {
    Undefined,               // 0x00
    Escape,                  // 0x01
    Num_1,                   // 0x02
    Num_2,                   // 0x03
    Num_3,                   // 0x04
    Num_4,                   // 0x05
    Num_5,                   // 0x06
    Num_6,                   // 0x07
    Num_7,                   // 0x08
    Num_8,                   // 0x09
    Num_9,                   // 0x0A
    Num_0,                   // 0x0B
    Minus,                   // 0x0C
    Equal,                   // 0x0D
    Backspace,               // 0x0E
    Tab,                     // 0x0F
    Q,                       // 0x10
    W,                       // 0x11
    E,                       // 0x12
    R,                       // 0x13
    T,                       // 0x14
    Y,                       // 0x15
    U,                       // 0x16
    I,                       // 0x17
    O,                       // 0x18
    P,                       // 0x19
    LeftBracket,             // 0x1A
    RightBracket,            // 0x1B
    Enter,                   // 0x1C
    LeftControl,             // 0x1D
    A,                       // 0x1E
    S,                       // 0x1F
    D,                       // 0x20
    F,                       // 0x21
    G,                       // 0x22
    H,                       // 0x23
    J,                       // 0x24
    K,                       // 0x25
    L,                       // 0x26
    Semicolon,               // 0x27
    Apostrophe,              // 0x28
    Grave,                   // 0x29
    LeftShift,               // 0x2A
    Backslash,               // 0x2B
    Z,                       // 0x2C
    X,                       // 0x2D
    C,                       // 0x2E
    V,                       // 0x2F
    B,                       // 0x30
    N,                       // 0x31
    M,                       // 0x32
    Comma,                   // 0x33
    Period,                  // 0x34
    Slash,                   // 0x35
    RightShift,              // 0x36
    KeypadAsterisk,          // 0x37
    LeftAlt,                 // 0x38
    Space,                   // 0x39
    CapsLock,                // 0x3A
    F1,                      // 0x3B
    F2,                      // 0x3C
    F3,                      // 0x3D
    F4,                      // 0x3E
    F5,                      // 0x3F
    F6,                      // 0x40
    F7,                      // 0x41
    F8,                      // 0x42
    F9,                      // 0x43
    F10,                     // 0x44
    NumLock,                 // 0x45
    ScrollLock,              // 0x46
    Keypad7,                 // 0x47
    Keypad8,                 // 0x48
    Keypad9,                 // 0x49
    KeypadMinus,             // 0x4A
    Keypad4,                 // 0x4B
    Keypad5,                 // 0x4C
    Keypad6,                 // 0x4D
    KeypadPlus,              // 0x4E
    Keypad1,                 // 0x4F
    Keypad2,                 // 0x50
    Keypad3,                 // 0x51
    Keypad0,                 // 0x52
    KeypadPeriod,            // 0x53
    Undefined,               // 0x54
    Undefined,               // 0x55
    NonUSBackslash,          // 0x56
    F11,                     // 0x57
    F12,                     // 0x58
    KeypadEqual,             // 0x59
    Undefined,               // 0x5A
    Undefined,               // 0x5B
    International6,          // 0x5C
    Undefined,               // 0x5D
    Undefined,               // 0x5E
    Undefined,               // 0x5F
    Undefined,               // 0x60
    Undefined,               // 0x61
    Undefined,               // 0x62
    Undefined,               // 0x63
    F13,                     // 0x64
    F14,                     // 0x65
    F15,                     // 0x66
    F16,                     // 0x67
    F17,                     // 0x68
    F18,                     // 0x69
    F19,                     // 0x6A
    F20,                     // 0x6B
    F21,                     // 0x6C
    F22,                     // 0x6D
    F23,                     // 0x6E
    Undefined,               // 0x6F
    International2,          // 0x70
    LANG2,                   // 0x71
    LANG1,                   // 0x72
    International1,          // 0x73
    Undefined,               // 0x74
    Undefined,               // 0x75
    F24,                     // 0x76
    LANG4,                   // 0x77
    LANG3,                   // 0x78
    International4,          // 0x79
    Undefined,               // 0x7A
    International5,          // 0x7B
    Undefined,               // 0x7C
    International3,          // 0x7D
    KeypadComma,             // 0x7E
    Undefined,               // 0x7F
    Undefined,               // 0x80
    Undefined,               // 0x81
    Undefined,               // 0x82
    Undefined,               // 0x83
    Undefined,               // 0x84
    Undefined,               // 0x85
    Undefined,               // 0x86
    Undefined,               // 0x87
    Undefined,               // 0x88
    Undefined,               // 0x89
    Undefined,               // 0x8A
    Undefined,               // 0x8B
    Undefined,               // 0x8C
    Undefined,               // 0x8D
    Undefined,               // 0x8E
    Undefined,               // 0x8F
    Undefined,               // 0x90
    Undefined,               // 0x91
    Undefined,               // 0x92
    Undefined,               // 0x93
    Undefined,               // 0x94
    Undefined,               // 0x95
    Undefined,               // 0x96
    Undefined,               // 0x97
    Undefined,               // 0x98
    Undefined,               // 0x99
    Undefined,               // 0x9A
    Undefined,               // 0x9B
    Undefined,               // 0x9C
    Undefined,               // 0x9D
    Undefined,               // 0x9E
    Undefined,               // 0x9F
    Undefined,               // 0xA0
    Undefined,               // 0xA1
    Undefined,               // 0xA2
    Undefined,               // 0xA3
    Undefined,               // 0xA4
    Undefined,               // 0xA5
    Undefined,               // 0xA6
    Undefined,               // 0xA7
    Undefined,               // 0xA8
    Undefined,               // 0xA9
    Undefined,               // 0xAA
    Undefined,               // 0xAB
    Undefined,               // 0xAC
    Undefined,               // 0xAD
    Undefined,               // 0xAE
    Undefined,               // 0xAF
    Undefined,               // 0xB0
    Undefined,               // 0xB1
    Undefined,               // 0xB2
    Undefined,               // 0xB3
    Undefined,               // 0xB4
    Undefined,               // 0xB5
    Undefined,               // 0xB6
    Undefined,               // 0xB7
    Undefined,               // 0xB8
    Undefined,               // 0xB9
    Undefined,               // 0xBA
    Undefined,               // 0xBB
    Undefined,               // 0xBC
    Undefined,               // 0xBD
    Undefined,               // 0xBE
    Undefined,               // 0xBF
    Undefined,               // 0xC0
    Undefined,               // 0xC1
    Undefined,               // 0xC2
    Undefined,               // 0xC3
    Undefined,               // 0xC4
    Undefined,               // 0xC5
    Undefined,               // 0xC6
    Undefined,               // 0xC7
    Undefined,               // 0xC8
    Undefined,               // 0xC9
    Undefined,               // 0xCA
    Undefined,               // 0xCB
    Undefined,               // 0xCC
    Undefined,               // 0xCD
    Undefined,               // 0xCE
    Undefined,               // 0xCF
    Undefined,               // 0xD0
    Undefined,               // 0xD1
    Undefined,               // 0xD2
    Undefined,               // 0xD3
    Undefined,               // 0xD4
    Undefined,               // 0xD5
    Undefined,               // 0xD6
    Undefined,               // 0xD7
    Undefined,               // 0xD8
    Undefined,               // 0xD9
    Undefined,               // 0xDA
    Undefined,               // 0xDB
    Undefined,               // 0xDC
    Undefined,               // 0xDD
    Undefined,               // 0xDE
    Undefined,               // 0xDF
    Undefined,               // 0xE0
    Undefined,               // 0xE1
    Undefined,               // 0xE2
    Undefined,               // 0xE3
    Undefined,               // 0xE4
    Undefined,               // 0xE5
    Undefined,               // 0xE6
    Undefined,               // 0xE7
    Undefined,               // 0xE8
    Undefined,               // 0xE9
    Undefined,               // 0xEA
    Undefined,               // 0xEB
    Undefined,               // 0xEC
    Undefined,               // 0xED
    Undefined,               // 0xEE
    Undefined,               // 0xEF
    Undefined,               // 0xF0
    Undefined,               // 0xF1
    Undefined,               // 0xF2
    Undefined,               // 0xF3
    Undefined,               // 0xF4
    Undefined,               // 0xF5
    Undefined,               // 0xF6
    Undefined,               // 0xF7
    Undefined,               // 0xF8
    Undefined,               // 0xF9
    Undefined,               // 0xFA
    Undefined,               // 0xFB
    Undefined,               // 0xFC
    Undefined,               // 0xFD
    Undefined,               // 0xFE
    ErrorRollOver,           // 0xFF
};

KeyEvent::KeyCode x11KeySymToKeyCode(KeySym keysym)
{
    switch (keysym)
    {
    case XK_BackSpace:
        return Backspace;
    case XK_Tab:
        return Tab;
    case XK_Linefeed:
    case XK_Return:
        return Enter;
    case XK_Clear:
        return Clear;
    case XK_Pause:
        return Pause;
    case XK_Scroll_Lock:
        return ScrollLock;
    case XK_Sys_Req:
        return SysReq;
    case XK_Escape:
        return Escape;
    case XK_Delete:
    case XK_KP_Delete:
        return Delete;
    case XK_space:
        return Space;

    case XK_Home:
    case XK_KP_Home:
        return Home;
    case XK_Left:
    case XK_KP_Left:
        return LeftArrow;
    case XK_Up:
    case XK_KP_Up:
        return UpArrow;
    case XK_Right:
    case XK_KP_Right:
        return RightArrow;
    case XK_Down:
    case XK_KP_Down:
        return DownArrow;
    case XK_Page_Up:
    case XK_KP_Page_Up:
        return PageUp;
    case XK_Page_Down:
    case XK_KP_Page_Down:
        return PageDown;
    case XK_End:
    case XK_KP_End:
        return End;
    case XK_Select:
        return Select;
    case XK_Print:
        return PrintScreen;
    case XK_Execute:
        return Execute;
    case XK_Insert:
    case XK_KP_Insert:
        return Insert;

    case XK_KP_Tab:
        return KeypadTab;
    case XK_KP_Enter:
        return KeypadEnter;
    case XK_KP_Space:
        return KeypadSpace;

    case XK_KP_F1:
    case XK_KP_F2:
    case XK_KP_F3:
    case XK_KP_F4:
        return static_cast<KeyEvent::KeyCode>(static_cast<uint8_t>(F1) + (keysym - XK_KP_F1));

    case XK_KP_Equal:
        return KeypadEqual;
    case XK_KP_Multiply:
        return KeypadAsterisk;
    case XK_KP_Add:
        return KeypadPlus;
    case XK_KP_Separator:
        return KeypadComma;
    case XK_KP_Subtract:
        return KeypadMinus;
    case XK_KP_Decimal:
        return KeypadDecimal;
    case XK_KP_Divide:
        return KeypadSlash;

    case XK_KP_0:
        return Keypad0;
    case XK_KP_1:
    case XK_KP_2:
    case XK_KP_3:
    case XK_KP_4:
    case XK_KP_5:
    case XK_KP_6:
    case XK_KP_7:
    case XK_KP_8:
    case XK_KP_9:
        return static_cast<KeyEvent::KeyCode>(static_cast<uint8_t>(Keypad1) + (keysym - XK_KP_1));

    case XK_F1:
    case XK_F2:
    case XK_F3:
    case XK_F4:
    case XK_F5:
    case XK_F6:
    case XK_F7:
    case XK_F8:
    case XK_F9:
    case XK_F10:
    case XK_F11:
    case XK_F12:
    case XK_F13:
    case XK_F14:
    case XK_F15:
    case XK_F16:
    case XK_F17:
    case XK_F18:
    case XK_F19:
    case XK_F20:
    case XK_F21:
    case XK_F22:
    case XK_F23:
    case XK_F24:
        return static_cast<KeyEvent::KeyCode>(static_cast<uint8_t>(F1) + (keysym - XK_F1));

    case XK_Shift_L:
        return LeftShift;
    case XK_Shift_R:
        return RightShift;
    case XK_Control_L:
        return LeftControl;
    case XK_Control_R:
        return RightControl;
    case XK_Caps_Lock:
        return CapsLock;
    case XK_Meta_L:
        return LeftGUI;
    case XK_Meta_R:
        return RightGUI;
    case XK_Alt_L:
        return LeftAlt;
    case XK_Alt_R:
        return RightAlt;
    case XK_Super_L:
        return LeftGUI;
    case XK_Super_R:
        return RightGUI;

    case XK_apostrophe:
        return Apostrophe;
    case XK_comma:
        return Comma;
    case XK_minus:
        return Minus;
    case XK_period:
        return Period;
    case XK_slash:
        return Slash;
    case XK_colon:
        return Apostrophe;
    case XK_semicolon:
        return Semicolon;
    case XK_at:
    case XK_bracketleft:
        return LeftBracket;
    case XK_backslash:
        return Backslash;
    case XK_bracketright:
        return RightBracket;
    case XK_grave:
        return Grave;

    default:
        return Undefined;
    }
}

}

namespace karin
{
KeyEvent::KeyCode x11ConvertKeyCode(unsigned int keycode)
{
    // TODO
    return x11ScanCodeToKeyCode[keycode - 8];
}

KeyEvent::KeyCode x11ConvertKeySym(KeySym keysym)
{
    if (keysym >= XK_0 && keysym <= XK_9)
    {
        return static_cast<KeyEvent::KeyCode>(static_cast<uint8_t>(Num_0) + (keysym - XK_0));
    }

    if (keysym >= XK_A && keysym <= XK_Z)
    {
        return static_cast<KeyEvent::KeyCode>(static_cast<uint8_t>(A) + (keysym - XK_A));
    }

    if (keysym >= XK_a && keysym <= XK_z)
    {
        return static_cast<KeyEvent::KeyCode>(static_cast<uint8_t>(A) + (keysym - XK_a));
    }

    return x11KeySymToKeyCode(keysym);
}

KeyEvent::Modifier x11ConvertModifier(unsigned int state)
{
    KeyEvent::Modifier modifier = KeyEvent::Modifier::None_;

    if (state & ShiftMask)
    {
        modifier |= KeyEvent::Modifier::Shift;
    }
    if (state & ControlMask)
    {
        modifier |= KeyEvent::Modifier::Control;
    }
    if (state & Mod1Mask)
    {
        modifier |= KeyEvent::Modifier::Alt;
    }
    if (state & Mod4Mask)
    {
        modifier |= KeyEvent::Modifier::Super;
    }

    return modifier;
}

std::string x11ConvertKeyChar(XEvent* event, XIC xic)
{
    char buf[32] = {};
    KeySym keysym;
    Status status;

    int count = Xutf8LookupString(xic, &event->xkey, buf, sizeof(buf) - 1, &keysym, &status);

    if (count > 0)
    {
        std::string str(buf, count);
        return str;
    } else
    {
        return "";
    }
}
}
