#ifndef CORE_SRC_SYSTEM_X11_X11_EVENT_H
#define CORE_SRC_SYSTEM_X11_X11_EVENT_H

#include <X11/Xlib.h>
#include <karin/system/event.h>

#include <optional>

namespace karin
{
std::optional<Event> translateX11Event(XEvent* event);
}

#endif //CORE_SRC_SYSTEM_X11_X11_EVENT_H
