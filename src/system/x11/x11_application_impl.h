#ifndef SRC_SYSTEM_X11_APPLICATION_IMPL_H
#define SRC_SYSTEM_X11_APPLICATION_IMPL_H

#include <X11/Xlib.h>

#include <application_impl.h>
#include <x11/window.h>
#include <map>
#include <queue>

namespace karin
{
class X11WindowImpl;

class X11ApplicationImpl : public IApplicationImpl
{
public:
    X11ApplicationImpl();
    ~X11ApplicationImpl() override;

    void addWindow(XlibWindow window, X11WindowImpl* impl);

    void shutdown() override;
    bool waitEvent(Event& event) override;

    void pushEvent(const Event& event)
    {
        m_eventQueue.push(event);
    }

    Display* display() const
    {
        return m_display;
    }

private:
    static int errorHandler(Display* display, XErrorEvent* error);

    Display* m_display;

    std::map<XlibWindow, X11WindowImpl*> m_windows;

    bool m_running = true;
    std::queue<Event> m_eventQueue;
};
} // karin

#endif //SRC_SYSTEM_X11_APPLICATION_IMPL_H