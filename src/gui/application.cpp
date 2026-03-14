#include "application.h"
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

std::shared_ptr<Window> Application::createWindow(const std::wstring& title, int x, int y, int width, int height)
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
    Event event;
    while (app.waitEvent(event)) { }
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