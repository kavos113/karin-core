#include "win_converter.h"

#include <array>

namespace
{
using namespace karin;
using enum KeyEvent::KeyCode;
std::array<KeyEvent::KeyCode, 256> windowsKeyMap = {
    Undefined,       // 0x00
    Undefined,       // 0x01 VK_LBUTTON
    Undefined,       // 0x02 VK_RBUTTON
    Undefined,       // 0x03 VK_CANCEL
    Undefined,       // 0x04 VK_MBUTTON
    Undefined,       // 0x05 VK_XBUTTON1
    Undefined,       // 0x06 VK_XBUTTON2
    Undefined,       // 0x07
    Backspace,       // 0x08 VK_BACK
    Tab,             // 0x09 VK_TAB
    Undefined,       // 0x0A
    Undefined,       // 0x0B
    Clear,           // 0x0C VK_CLEAR
    Enter,           // 0x0D VK_RETURN
    Undefined,       // 0x0E
    Undefined,       // 0x0F
    LeftShift,       // 0x10 VK_SHIFT
    LeftControl,     // 0x11 VK_CONTROL
    LeftAlt,         // 0x12 VK_MENU
    Pause,           // 0x13 VK_PAUSE
    CapsLock,        // 0x14 VK_CAPITAL
    Undefined,       // 0x15 VK_KANA / VK_HANGUL
    Undefined,       // 0x16 VK_IME_ON
    Undefined,       // 0x17 VK_JUNJA
    Undefined,       // 0x18 VK_FINAL
    Undefined,       // 0x19 VK_HANJA / VK_KANJI
    Undefined,       // 0x1A VK_IME_OFF
    Escape,          // 0x1B VK_ESCAPE
    Undefined,       // 0x1C VK_CONVERT
    Undefined,       // 0x1D VK_NONCONVERT
    Undefined,       // 0x1E VK_ACCEPT
    Undefined,       // 0x1F VK_MODECHANGE
    Space,           // 0x20 VK_SPACE
    PageUp,          // 0x21 VK_PRIOR
    PageDown,        // 0x22 VK_NEXT
    End,             // 0x23 VK_END
    Home,            // 0x24 VK_HOME
    LeftArrow,       // 0x25 VK_LEFT
    UpArrow,         // 0x26 VK_UP
    RightArrow,      // 0x27 VK_RIGHT
    DownArrow,       // 0x28 VK_DOWN
    Select,          // 0x29 VK_SELECT
    Undefined,       // 0x2A VK_PRINT
    Execute,         // 0x2B VK_EXECUTE
    PrintScreen,     // 0x2C VK_SNAPSHOT
    Insert,          // 0x2D VK_INSERT
    Delete,          // 0x2E VK_DELETE
    Help,            // 0x2F VK_HELP
    Num_0,           // 0x30 '0'
    Num_1,           // 0x31 '1'
    Num_2,           // 0x32 '2'
    Num_3,           // 0x33 '3'
    Num_4,           // 0x34 '4'
    Num_5,           // 0x35 '5'
    Num_6,           // 0x36 '6'
    Num_7,           // 0x37 '7'
    Num_8,           // 0x38 '8'
    Num_9,           // 0x39 '9'
    Undefined,       // 0x3A
    Undefined,       // 0x3B
    Undefined,       // 0x3C
    Undefined,       // 0x3D
    Undefined,       // 0x3E
    Undefined,       // 0x3F
    Undefined,       // 0x40
    A,               // 0x41 'A'
    B,               // 0x42 'B'
    C,               // 0x43 'C'
    D,               // 0x44 'D'
    E,               // 0x45 'E'
    F,               // 0x46 'F'
    G,               // 0x47 'G'
    H,               // 0x48 'H'
    I,               // 0x49 'I'
    J,               // 0x4A 'J'
    K,               // 0x4B 'K'
    L,               // 0x4C 'L'
    M,               // 0x4D 'M'
    N,               // 0x4E 'N'
    O,               // 0x4F 'O'
    P,               // 0x50 'P'
    Q,               // 0x51 'Q'
    R,               // 0x52 'R'
    S,               // 0x53 'S'
    T,               // 0x54 'T'
    U,               // 0x55 'U'
    V,               // 0x56 'V'
    W,               // 0x57 'W'
    X,               // 0x58 'X'
    Y,               // 0x59 'Y'
    Z,               // 0x5A 'Z'
    LeftGUI,         // 0x5B VK_LWIN
    RightGUI,        // 0x5C VK_RWIN
    Application,     // 0x5D VK_APPS
    Undefined,       // 0x5E
    Undefined,       // 0x5F VK_SLEEP
    Keypad0,         // 0x60 VK_NUMPAD0
    Keypad1,         // 0x61 VK_NUMPAD1
    Keypad2,         // 0x62 VK_NUMPAD2
    Keypad3,         // 0x63 VK_NUMPAD3
    Keypad4,         // 0x64 VK_NUMPAD4
    Keypad5,         // 0x65 VK_NUMPAD5
    Keypad6,         // 0x66 VK_NUMPAD6
    Keypad7,         // 0x67 VK_NUMPAD7
    Keypad8,         // 0x68 VK_NUMPAD8
    Keypad9,         // 0x69 VK_NUMPAD9
    KeypadAsterisk,  // 0x6A VK_MULTIPLY
    KeypadPlus,      // 0x6B VK_ADD
    Undefined,       // 0x6C VK_SEPARATOR
    KeypadMinus,     // 0x6D VK_SUBTRACT
    KeypadPeriod,    // 0x6E VK_DECIMAL
    KeypadSlash,     // 0x6F VK_DIVIDE
    F1,              // 0x70 VK_F1
    F2,              // 0x71 VK_F2
    F3,              // 0x72 VK_F3
    F4,              // 0x73 VK_F4
    F5,              // 0x74 VK_F5
    F6,              // 0x75 VK_F6
    F7,              // 0x76 VK_F7
    F8,              // 0x77 VK_F8
    F9,              // 0x78 VK_F9
    F10,             // 0x79 VK_F10
    F11,             // 0x7A VK_F11
    F12,             // 0x7B VK_F12
    F13,             // 0x7C VK_F13
    F14,             // 0x7D VK_F14
    F15,             // 0x7E VK_F15
    F16,             // 0x7F VK_F16
    F17,             // 0x80 VK_F17
    F18,             // 0x81 VK_F18
    F19,             // 0x82 VK_F19
    F20,             // 0x83 VK_F20
    F21,             // 0x84 VK_F21
    F22,             // 0x85 VK_F22
    F23,             // 0x86 VK_F23
    F24,             // 0x87 VK_F24
    Undefined,       // 0x88
    Undefined,       // 0x89
    Undefined,       // 0x8A
    Undefined,       // 0x8B
    Undefined,       // 0x8C
    Undefined,       // 0x8D
    Undefined,       // 0x8E
    Undefined,       // 0x8F
    Undefined,       // 0x90 VK_NUMLOCK
    ScrollLock,      // 0x91 VK_SCROLL
    Undefined,       // 0x92
    Undefined,       // 0x93
    Undefined,       // 0x94
    Undefined,       // 0x95
    Undefined,       // 0x96
    Undefined,       // 0x97
    Undefined,       // 0x98
    Undefined,       // 0x99
    Undefined,       // 0x9A
    Undefined,       // 0x9B
    Undefined,       // 0x9C
    Undefined,       // 0x9D
    Undefined,       // 0x9E
    Undefined,       // 0x9F
    LeftShift,       // 0xA0 VK_LSHIFT
    RightShift,      // 0xA1 VK_RSHIFT
    LeftControl,     // 0xA2 VK_LCONTROL
    RightControl,    // 0xA3 VK_RCONTROL
    LeftAlt,         // 0xA4 VK_LMENU
    RightAlt,        // 0xA5 VK_RMENU
    Undefined,       // 0xA6 VK_BROWSER_BACK
    Undefined,       // 0xA7 VK_BROWSER_FORWARD
    Undefined,       // 0xA8 VK_BROWSER_REFRESH
    Undefined,       // 0xA9 VK_BROWSER_STOP
    Undefined,       // 0xAA VK_BROWSER_SEARCH
    Undefined,       // 0xAB VK_BROWSER_FAVORITES
    Undefined,       // 0xAC VK_BROWSER_HOME
    Mute,            // 0xAD VK_VOLUME_MUTE
    VolumeUp,        // 0xAE VK_VOLUME_UP
    VolumeDown,      // 0xAF VK_VOLUME_DOWN
    Undefined,       // 0xB0 VK_MEDIA_NEXT_TRACK
    Undefined,       // 0xB1 VK_MEDIA_PREV_TRACK
    Undefined,       // 0xB2 VK_MEDIA_STOP
    Undefined,       // 0xB3 VK_MEDIA_PLAY_PAUSE
    Undefined,       // 0xB4 VK_LAUNCH_MAIL
    Undefined,       // 0xB5 VK_LAUNCH_MEDIA_SELECT
    Undefined,       // 0xB6 VK_LAUNCH_APP1
    Undefined,       // 0xB7 VK_LAUNCH_APP2
    Undefined,       // 0xB8
    Undefined,       // 0xB9
    Undefined,       // 0xBA VK_OEM_1
    Undefined,       // 0xBB VK_OEM_PLUS
    Undefined,       // 0xBC VK_OEM_COMMA
    Undefined,       // 0xBD VK_OEM_MINUS
    Undefined,       // 0xBE VK_OEM_PERIOD
    Undefined,       // 0xBF VK_OEM_2
    Undefined,       // 0xC0 VK_OEM_3
    Undefined,       // 0xC1
    Undefined,       // 0xC2
    Undefined,       // 0xC3 VK_GAMEPAD_A
    Undefined,       // 0xC4 VK_GAMEPAD_B
    Undefined,       // 0xC5 VK_GAMEPAD_X
    Undefined,       // 0xC6 VK_GAMEPAD_Y
    Undefined,       // 0xC7 VK_GAMEPAD_RIGHT_SHOULDER
    Undefined,       // 0xC8 VK_GAMEPAD_LEFT_SHOULDER
    Undefined,       // 0xC9 VK_GAMEPAD_LEFT_TRIGGER
    Undefined,       // 0xCA VK_GAMEPAD_RIGHT_TRIGGER
    Undefined,       // 0xCB VK_GAMEPAD_DPAD_UP
    Undefined,       // 0xCC VK_GAMEPAD_DPAD_DOWN
    Undefined,       // 0xCD VK_GAMEPAD_DPAD_LEFT
    Undefined,       // 0xCE VK_GAMEPAD_DPAD_RIGHT
    Undefined,       // 0xCF VK_GAMEPAD_MENU
    Undefined,       // 0xD0 VK_GAMEPAD_VIEW
    Undefined,       // 0xD1 VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON
    Undefined,       // 0xD2 VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON
    Undefined,       // 0xD3 VK_GAMEPAD_LEFT_THUMBSTICK_UP
    Undefined,       // 0xD4 VK_GAMEPAD_LEFT_THUMBSTICK_DOWN
    Undefined,       // 0xD5 VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT
    Undefined,       // 0xD6 VK_GAMEPAD_LEFT_THUMBSTICK_LEFT
    Undefined,       // 0xD7 VK_GAMEPAD_RIGHT_THUMBSTICK_UP
    Undefined,       // 0xD8 VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN
    Undefined,       // 0xD9 VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT
    Undefined,       // 0xDA VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT
    Undefined,       // 0xDB VK_OEM_4
    Undefined,       // 0xDC VK_OEM_5
    Undefined,       // 0xDD VK_OEM_6
    Undefined,       // 0xDE VK_OEM_7
    Undefined,       // 0xDF VK_OEM_8
    Undefined,       // 0xE0
    Undefined,       // 0xE1
    Undefined,       // 0xE2 VK_OEM_102
    Undefined,       // 0xE3
    Undefined,       // 0xE4
    Undefined,       // 0xE5 VK_PROCESSKEY
    Undefined,       // 0xE6
    Undefined,       // 0xE7 VK_PACKET
    Undefined,       // 0xE8
    Undefined,       // 0xE9
    Undefined,       // 0xEA
    Undefined,       // 0xEB
    Undefined,       // 0xEC
    Undefined,       // 0xED
    Undefined,       // 0xEE
    Undefined,       // 0xEF
    Undefined,       // 0xF0
    Undefined,       // 0xF1
    Undefined,       // 0xF2
    Undefined,       // 0xF3
    Undefined,       // 0xF4
    Undefined,       // 0xF5
    Undefined,       // 0xF6 VK_ATTN
    CrSel,           // 0xF7 VK_CRSEL
    ExSel,           // 0xF8 VK_EXSEL
    Undefined,       // 0xF9 VK_EREOF
    Undefined,       // 0xFA VK_PLAY
    Undefined,       // 0xFB VK_ZOOM
    Undefined,       // 0xFC VK_NONAME
    Undefined,       // 0xFD VK_PA1
    Undefined        // 0xFE VK_OEM_CLEAR
};
}

namespace karin
{

}
