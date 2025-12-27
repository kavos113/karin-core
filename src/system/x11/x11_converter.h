#ifndef SRC_SYSTEM_X11_X11_CONVERTER_H
#define SRC_SYSTEM_X11_X11_CONVERTER_H

#include <karin/system/event.h>

#include <X11/Xlib.h>

namespace karin
{
KeyEvent::KeyCode x11ConvertKeyCode(unsigned int keycode);
KeyEvent::KeyCode x11ConvertKeySym(KeySym keysym);
KeyEvent::Modifier x11ConvertModifier(unsigned int state);
std::string x11ConvertKeyChar(XEvent* event, XIC xic);
}

#endif //SRC_SYSTEM_X11_X11_CONVERTER_H