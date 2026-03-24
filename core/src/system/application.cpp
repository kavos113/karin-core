#include <karin/system/application.h>

#include "platform.h"
#include "application_impl.h"

namespace karin
{
Application& Application::instance()
{
    static Application instance;

    return instance;
}

bool Application::waitEvent(Event &event) const
{
    return m_impl->waitEvent(event);
}

std::unique_ptr<Window> Application::createWindow(const std::string& title, int x, int y, int width, int height)
{
    return std::make_unique<Window>(
        m_impl.get(),
        title,
        x,
        y,
        width,
        height
    );
}

std::unique_ptr<Window> Application::createWindow(const std::string& title, Rectangle rect)
{
    return std::make_unique<Window>(
        m_impl.get(),
        title,
        rect
    );
}

Application::Application()
{
    m_impl = createApplicationImpl();
}
}