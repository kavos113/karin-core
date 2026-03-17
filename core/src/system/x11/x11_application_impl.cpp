#include "x11_application_impl.h"

#include <iostream>
#include <stdexcept>

#include "x11_window_impl.h"
#include "x11_context.h"

namespace karin
{
X11ApplicationImpl::X11ApplicationImpl()
{
    XSetErrorHandler(errorHandler);
    XSynchronize(X11Context::instance().display(), True);
}

X11ApplicationImpl::~X11ApplicationImpl()
{
    XCloseDisplay(X11Context::instance().display());
}

void X11ApplicationImpl::addWindow(XlibWindow window, X11WindowImpl* impl)
{
    m_windows[window] = impl;
}

bool X11ApplicationImpl::waitEvent(Event& event)
{
    if (!m_running)
        return false;

    if (!m_eventQueue.empty())
    {
        event = m_eventQueue.front();
        m_eventQueue.pop();
        return true;
    }

    XEvent xevent;
    XNextEvent(X11Context::instance().display(), &xevent);

    m_windows[xevent.xany.window]->handleEvent(xevent);

    if (!m_eventQueue.empty())
    {
        event = m_eventQueue.front();
        m_eventQueue.pop();
        return true;
    }

    return true;
}

void X11ApplicationImpl::shutdown()
{
    m_running = false;
}

int X11ApplicationImpl::errorHandler(Display* display, XErrorEvent* error)
{
    char errorText[256];
    XGetErrorText(display, error->error_code, errorText, sizeof(errorText));

    std::cerr << "X11 Error: " << errorText << " (request code: " << error->request_code
        << ", minor code: " << error->minor_code << ")" << std::endl;

    return 0;
}
} // karin