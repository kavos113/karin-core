#ifndef KARIN_WIN_EVENT_H
#define KARIN_WIN_EVENT_H

#include <windows.h>
#include <karin/system/event.h>

#include <optional>

#define WM_KARIN_ACTION (WM_USER + 1)

namespace karin
{
std::optional<Event> translateWinEvent(UINT message, WPARAM wParam, LPARAM lParam);
} // karin

#endif //KARIN_WIN_EVENT_H