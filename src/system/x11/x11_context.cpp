#include "x11_context.h"

namespace karin
{
X11Context& X11Context::instance()
{
    static X11Context ctx;
    return ctx;
}

Display* X11Context::display() const
{
    return m_display;
}

X11Context::X11Context()
{
    m_display = XOpenDisplay(nullptr);
}

X11Context::~X11Context()
{
    if (m_display)
    {
        XCloseDisplay(m_display);
        m_display = nullptr;
    }
}
} // karin