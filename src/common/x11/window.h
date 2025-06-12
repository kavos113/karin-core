#ifndef SRC_COMMON_X11_WINDOW_H
#define SRC_COMMON_X11_WINDOW_H

#include <X11/Xlib.h>

// conflict between Xlib Window and Window from system/window.h
using XlibWindow = Window;

#endif //SRC_COMMON_X11_WINDOW_H
