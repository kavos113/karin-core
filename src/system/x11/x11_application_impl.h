#ifndef SRC_SYSTEM_X11_APPLICATION_IMPL_H
#define SRC_SYSTEM_X11_APPLICATION_IMPL_H

#include <X11/Xlib.h>

#include <application_impl.h>
#include <x11/window.h>
#include <map>

namespace karin
{
class X11WindowImpl;

class X11ApplicationImpl : public IApplicationImpl
{
public:
    X11ApplicationImpl();
    ~X11ApplicationImpl() override;

    void addWindow(XlibWindow window, X11WindowImpl* impl);

    void run() override;
    void shutdown() override;

    Display* display() const
    {
        return m_display;
    }

private:
    static int errorHandler(Display* display, XErrorEvent* error);

    Display* m_display;

    std::map<XlibWindow, X11WindowImpl*> m_windows;

    bool m_running = true;
};
} // karin

#endif //SRC_SYSTEM_X11_APPLICATION_IMPL_H