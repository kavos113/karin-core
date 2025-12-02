#ifndef SRC_SYSTEM_WINDOWS_WIN_CONVERTER_H
#define SRC_SYSTEM_WINDOWS_WIN_CONVERTER_H

#include <karin/system/event.h>

#include <windows.h>

namespace karin
{

KeyEvent::KeyCode winVirtualKeyToKeyCode(WPARAM wParam);
KeyEvent::KeyCode winScanCodeToKeyCode(LPARAM lParam);
KeyEvent::Modifier getWinModifierState();
}

#endif //SRC_SYSTEM_WINDOWS_WIN_CONVERTER_H