#include <karin/system/window.h>

#include "platform.h"

namespace karin
{
Window::Window(const std::wstring &title, int x, int y, int width, int height)
{
    m_impl = createWindowImpl(title, x, y, width, height);
}

Window::~Window()
{
}

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
} // karin