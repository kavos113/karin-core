#ifndef KARIN_WIN_EVENT_H
#define KARIN_WIN_EVENT_H

#include <windows.h>
#include <karin/system/event.h>

namespace karin
{
std::unique_ptr<Event> translateWinEvent(UINT message, WPARAM wParam, LPARAM lParam);
} // karin

#endif //KARIN_WIN_EVENT_H