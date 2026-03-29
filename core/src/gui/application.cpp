#include <karin/gui/application.h>
#include "application_context.h"

namespace karin::gui
{
static ApplicationContext *s_appContext = nullptr;

Application::Application()
{
    if (s_appContext)
    {
        throw std::runtime_error("Application already exists");
    }

    m_context = std::make_unique<ApplicationContext>();
    s_appContext = m_context.get();
}

Application::~Application()
{
    s_appContext = nullptr;
}

std::shared_ptr<Window> Application::createWindow(const std::string& title, int x, int y, int width, int height)
{
    auto window = std::make_shared<Window>(title, x, y, width, height);
    m_windows.push_back(window);

    return window;
}

void Application::run()
{
    for (auto& window : m_windows)
    {
        window->beforeRun();
    }

    karin::Application& app = karin::Application::instance();
    EventPayload event;
    while (app.waitEvent(event))
    {
        if (std::holds_alternative<std::monostate>(event.event))
        {
            continue;
        }

        karin::Window* window = app.findWindow(event.windowId);
        if (!window)
        {
            continue;
        }

        void* userData = window->userData();
        if (userData)
        {
            auto* guiWindow = static_cast<Window*>(userData);
            guiWindow->dispatchEvent(event.event);
        }
    }
}

ApplicationContext& getAppContext()
{
    if (!s_appContext)
    {
        throw std::runtime_error("Application context is not initialized");
    }
    return *s_appContext;
}
} // karin::gui