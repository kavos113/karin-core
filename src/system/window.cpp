#include <karin/system/window.h>

#include "platform.h"

namespace karin
{

Window::Window(IApplicationImpl *applicationImpl, const std::wstring &title, int x, int y, int width, int height)
    : m_rect(Rectangle(
        static_cast<float>(x),
        static_cast<float>(y),
        static_cast<float>(width),
        static_cast<float>(height)
    ))
    , m_impl(createWindowImpl(title, x, y, width, height, applicationImpl))
{
}

Window::Window(IApplicationImpl *applicationImpl, const std::wstring &title, Rectangle rect)
    : m_rect(rect)
    , m_impl(createWindowImpl(
        title,
        static_cast<int>(rect.pos.x),
        static_cast<int>(rect.pos.y),
        static_cast<int>(rect.size.width),
        static_cast<int>(rect.size.height),
        applicationImpl
    ))
{
}

Window::~Window() = default;

void Window::setStatus(ShowStatus status)
{
    switch (status)
    {
        case ShowStatus::HIDE:
            m_impl->hide();
            break;
        case ShowStatus::SHOW:
            m_impl->show();
            break;
        case ShowStatus::MINIMIZE:
            m_impl->minimize();
            break;
        case ShowStatus::MAXIMIZE:
            m_impl->maximize();
            break;
    }
    m_showStatus = status;
}

Window::ShowStatus Window::status() const
{
    return m_showStatus;
}

void* Window::handle() const
{
    return m_impl->handle();
}

void Window::setPosition(Point pos)
{
    m_rect.pos = pos;
    m_impl->setPosition(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

void Window::setSize(Size size)
{
    m_rect.size = size;
    m_impl->setSize(static_cast<int>(size.width), static_cast<int>(size.height));
}

void Window::setRect(Rectangle rect)
{
    m_rect = rect;
    m_impl->setRect(
        static_cast<int>(rect.pos.x),
        static_cast<int>(rect.pos.y),
        static_cast<int>(rect.size.width),
        static_cast<int>(rect.size.height)
    );
}

Point Window::position() const
{
    return m_rect.pos;
}

Size Window::size() const
{
    return m_rect.size;
}

Rectangle Window::rect() const
{
    return m_rect;
}

void Window::setOnPaint(std::function<void()> onPaint)
{
    m_impl->setOnPaint(std::move(onPaint));
}

void Window::setOnResize(std::function<void(Size)> onResize)
{
    m_impl->setOnResize(std::move(onResize));
}
} // karin