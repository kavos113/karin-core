#include "x11_window_impl.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <X11/Xutil.h>
#include <X11/Xlocale.h>

#include <utils/string.h>

#include "x11_converter.h"

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
    : m_appImpl(appImpl)
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
        ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask
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

    if (setlocale(LC_ALL, "") == nullptr)
    {
        throw std::runtime_error("failed to set locale");
    }

    if (!XSetLocaleModifiers(""))
    {
        throw std::runtime_error("failed to set x locale");
    }

    m_xim = XOpenIM(m_display, nullptr, nullptr, nullptr);
    if (!m_xim)
    {
        throw std::runtime_error("failed to open input method");
    }

    m_xic = XCreateIC(
        m_xim,
        XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
        XNClientWindow, m_window,
        XNFocusWindow, m_window,
        nullptr
    );
    if (!m_xic)
    {
        throw std::runtime_error("failed to create input context");
    }
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
    std::optional<Event> translatedEvent = translateX11Event(&const_cast<XEvent&>(event));
    if (translatedEvent.has_value())
    {
        m_appImpl->pushEvent(translatedEvent.value());
    }

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

    case KeyPress:
        m_appImpl->pushEvent(KeyTypeEvent(x11ConvertKeyChar(&const_cast<XEvent&>(event), m_xic)));
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

std::optional<Event> X11WindowImpl::translateX11Event(XEvent* event)
{
    switch (event->type)
    {
    case ButtonPress:
    {
        int x = event->xbutton.x_root;
        int y = event->xbutton.y_root;
        switch (event->xbutton.button)
        {
        case Button1:
            return MouseButtonEvent(
                MouseButtonEvent::Type::ButtonPress_,
                MouseButtonEvent::Button::Left,
                x, y
            );
        case Button2:
            return MouseButtonEvent(
                MouseButtonEvent::Type::ButtonPress_,
                MouseButtonEvent::Button::Middle,
                x, y
            );
        case Button3:
            return MouseButtonEvent(
                MouseButtonEvent::Type::ButtonPress_,
                MouseButtonEvent::Button::Right,
                x, y
            );
        case Button4:
            return MouseWheelEvent(
                SCROLL_DELTA,
                x, y
            );
        case Button5:
            return MouseWheelEvent(
                -SCROLL_DELTA,
                x, y
            );
        default:
            break;
        }
        break;
    }

    case ButtonRelease:
    {
        int x = event->xbutton.x_root;
        int y = event->xbutton.y_root;
        switch (event->xbutton.button)
        {
        case Button1:
            return MouseButtonEvent(
                MouseButtonEvent::Type::ButtonRelease_,
                MouseButtonEvent::Button::Left,
                x, y
            );
        case Button2:
            return MouseButtonEvent(
                MouseButtonEvent::Type::ButtonRelease_,
                MouseButtonEvent::Button::Middle,
                x, y
            );
        case Button3:
            return MouseButtonEvent(
                MouseButtonEvent::Type::ButtonRelease_,
                MouseButtonEvent::Button::Right,
                x, y
            );
        default:
            break;
        }
        break;
    }

    case KeyPress:
    {
        KeySym keysym = XLookupKeysym(&event->xkey, 0);
        return KeyEvent(
            KeyEvent::Type::KeyPress_,
            x11ConvertKeyCode(event->xkey.keycode),
            x11ConvertKeySym(keysym),
            x11ConvertModifier(event->xkey.state)
        );
    }

    case KeyRelease:
    {
        KeySym keysym = XLookupKeysym(&event->xkey, 0);
        return KeyEvent(
            KeyEvent::Type::KeyRelease_,
            x11ConvertKeyCode(event->xkey.keycode),
            x11ConvertKeySym(keysym),
            x11ConvertModifier(event->xkey.state)
        );
    }

    default:
        return std::nullopt;
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

void X11WindowImpl::setOnStartResize(std::function<void()> onStartResize)
{
    m_onStartResize = std::move(onStartResize);
}

void X11WindowImpl::setOnFinishResize(std::function<void()> onFinishResize)
{
    m_onFinishResize = std::move(onFinishResize);
}

Window::NativeHandle X11WindowImpl::handle() const
{
    return Window::NativeHandle{
        .display = m_display,
        .window = static_cast<uint64_t>(m_window)
    };
}
} // karin