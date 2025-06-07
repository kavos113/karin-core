#include "x11_application_impl.h"

#include <stdexcept>

#include "x11_window_impl.h"

namespace karin
{

X11ApplicationImpl::X11ApplicationImpl()
{
    m_display = XOpenDisplay(nullptr);
    if (!m_display)
    {
        throw std::runtime_error("Failed to open X11 display");
    }
}

X11ApplicationImpl::~X11ApplicationImpl()
{
    XCloseDisplay(m_display);
}

void X11ApplicationImpl::addWindow(Window window, X11WindowImpl *impl)
{
    m_windows[window] = impl;
}

void X11ApplicationImpl::run()
{
    XEvent event;
    while (m_running)
    {
        XNextEvent(m_display, &event);

        m_windows[event.xany.window]->handleEvent(event);
    }
}

void X11ApplicationImpl::shutdown()
{
    m_running = false;
}

} // karin