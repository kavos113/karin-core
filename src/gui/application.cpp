#include "application.h"

namespace karin::gui
{
Application::Application()
{
    m_textEngine = std::make_unique<TextEngine>();
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
} // karin::gui