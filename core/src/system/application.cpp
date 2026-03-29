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

bool Application::waitEvent(EventPayload &event) const
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

WindowID Application::registerWindow(Window* window)
{
    WindowID id = m_nextWindowID++;
    m_windowRegistry[id] = window;
    return id;
}

void Application::unregisterWindow(WindowID id)
{
    m_windowRegistry.erase(id);
}

Window* Application::findWindow(WindowID id) const
{
    if (auto it = m_windowRegistry.find(id); it != m_windowRegistry.end())
    {
        return it->second;
    }
    return nullptr;
}

Application::Application()
{
    m_impl = createApplicationImpl();
}
}