#include "x11_window_impl.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <X11/Xutil.h>

#include <utils/string.h>

namespace karin
{
X11WindowImpl::X11WindowImpl(
    const std::wstring& title,
    int x,
    int y,
    int width,
    int height,
    X11ApplicationImpl* appImpl
)
{
    m_display = appImpl->display();

    XSizeHints* sizeHints = XAllocSizeHints();
    if (!sizeHints)
    {
        throw std::runtime_error("Failed to allocate size hints");
    }

    XWMHints* wmHints = XAllocWMHints();
    if (!wmHints)
    {
        XFree(sizeHints);
        throw std::runtime_error("Failed to allocate window manager hints");
    }

    XClassHint* classHint = XAllocClassHint();
    if (!classHint)
    {
        XFree(sizeHints);
        XFree(wmHints);
        throw std::runtime_error("Failed to allocate class hint");
    }

    m_window = XCreateSimpleWindow(
        m_display,
        DefaultRootWindow(m_display),
        x, y, width, height,
        0,
        BlackPixel(m_display, 0),
        WhitePixel(m_display, 0)
    );

    sizeHints->flags = PPosition | PSize;

    XTextProperty windowName;
    std::string titleString = toString(title);
    const char* titleStr = titleString.c_str();
    if (!XStringListToTextProperty(const_cast<char**>(&titleStr), 1, &windowName))
    {
        XFree(sizeHints);
        XFree(wmHints);
        XFree(classHint);
        throw std::runtime_error("Failed to convert title to text property");
    }

    wmHints->flags = InputHint | StateHint;
    wmHints->input = True;
    wmHints->initial_state = NormalState;

    classHint->res_name = const_cast<char*>(titleStr);
    classHint->res_class = const_cast<char*>(titleStr);

    XSetWMProperties(
        m_display,
        m_window,
        &windowName,
        nullptr,
        nullptr, 0,
        sizeHints,
        wmHints,
        classHint
    );

    Atom netWmName = XInternAtom(m_display, "_NET_WM_NAME", False);
    Atom utf8String = XInternAtom(m_display, "UTF8_STRING", False);
    XChangeProperty(
        m_display,
        m_window,
        netWmName,
        utf8String,
        8,
        PropModeReplace,
        reinterpret_cast<const unsigned char*>(titleStr),
        static_cast<int>(title.length() * sizeof(wchar_t))
    );

    XSelectInput(
        m_display,
        m_window,
        ExposureMask | StructureNotifyMask | KeyPressMask
    );

    Atom wmDelete = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(m_display, m_window, &wmDelete, 1);

    const uint64_t valueMask = 0;
    XGCValues values;
    m_gc = XCreateGC(m_display, m_window, valueMask, &values);
    if (!m_gc)
    {
        XFree(sizeHints);
        XFree(wmHints);
        XFree(classHint);
        throw std::runtime_error("Failed to create graphics context");
    }
    XSetForeground(m_display, m_gc, BlackPixel(m_display, 0));

    XFree(classHint);
    XFree(wmHints);
    XFree(sizeHints);

    appImpl->addWindow(m_window, this);

    m_onClose = [this, appImpl]
    {
        hide();
        appImpl->shutdown();
    };
}

X11WindowImpl::~X11WindowImpl()
{
    if (m_gc)
    {
        XFreeGC(m_display, m_gc);
    }
    if (m_window)
    {
        XDestroyWindow(m_display, m_window);
    }
    XFlush(m_display);
}

void X11WindowImpl::handleEvent(const XEvent& event)
{
    switch (event.type)
    {
    case Expose:
        std::call_once(m_applyStatusFlag, &X11WindowImpl::applyStatus, this);
        if (m_onPaint)
        {
            bool ret = m_onPaint();
            if (!ret)
            {
                m_onPaint(); // redraw
            }
        }
        break;

    case ConfigureNotify:
        if (m_onResize)
        {
            m_onResize(
                Size(
                    event.xconfigure.width,
                    event.xconfigure.height
                )
            );
        }
        break;

    case ClientMessage:
        if (event.xclient.data.l[0] == XInternAtom(m_display, "WM_DELETE_WINDOW", False))
        {
            m_onClose();
        }
        break;

    default:
        break;
    }
}

void X11WindowImpl::show()
{
    if (!m_window)
    {
        return;
    }

    XMapWindow(m_display, m_window);
}

void X11WindowImpl::hide()
{
    if (!m_window)
    {
        return;
    }

    XUnmapWindow(m_display, m_window);
}

void X11WindowImpl::minimize()
{
    if (!m_window)
    {
        return;
    }

    XIconifyWindow(m_display, m_window, DefaultScreen(m_display));
    XFlush(m_display);

    m_status = X11ShowStatus::Minimize;
}

void X11WindowImpl::maximize()
{
    XMapWindow(m_display, m_window);

    if (!m_window)
    {
        return;
    }

    XEvent event = {};

    event.type = ClientMessage;
    event.xclient.window = m_window;
    event.xclient.message_type = XInternAtom(m_display, "_NET_WM_STATE", False);
    event.xclient.format = 32;
    event.xclient.data.l[0] = 1; // _NET_WM_STATE_ADD
    event.xclient.data.l[1] = XInternAtom(m_display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
    event.xclient.data.l[2] = XInternAtom(m_display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
    event.xclient.data.l[3] = 0; // no specific window

    XSendEvent(
        m_display,
        DefaultRootWindow(m_display),
        False,
        SubstructureRedirectMask | SubstructureNotifyMask,
        &event
    );

    XFlush(m_display);

    m_status = X11ShowStatus::Maximize;
}

void X11WindowImpl::applyStatus()
{
    if (m_status == X11ShowStatus::Maximize)
    {
        maximize();
    }
    else if (m_status == X11ShowStatus::Minimize)
    {
        minimize();
    }
}

void X11WindowImpl::setPosition(int x, int y)
{
    XMoveWindow(m_display, m_window, x, y);
}

void X11WindowImpl::setSize(int width, int height)
{
    XResizeWindow(m_display, m_window, width, height);
}

void X11WindowImpl::setRect(int x, int y, int width, int height)
{
    XMoveResizeWindow(m_display, m_window, x, y, width, height);
}

void X11WindowImpl::setOnPaint(std::function<bool()> onPaint)
{
    m_onPaint = std::move(onPaint);
}

void X11WindowImpl::setOnResize(std::function<void(Size)> onResize)
{
    m_onResize = std::move(onResize);
}

Window::NativeHandle X11WindowImpl::handle() const
{
    return Window::NativeHandle{
        .display = m_display,
        .window = static_cast<uint64_t>(m_window)
    };
}
} // karin