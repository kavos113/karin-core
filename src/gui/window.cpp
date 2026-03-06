#include "window.h"

#include <karin/system/application.h>
#include <karin/common/color/color.h>

namespace karin::gui
{
Window::Window()
{
    Application& app = Application::instance();
    m_window = app.createWindow(L"Karin GUI Window", 100, 100, 800, 600);

    m_renderer = std::make_unique<Renderer>(m_window.get());
    m_renderer->setClearColor(Color(Color::White));
}

void Window::run()
{
    m_rootView->calculateLayout();

    m_renderer->addDrawCommand(
        [this](GraphicsContext& gc)
        {            // For debugging
            m_rootView->draw(gc);
        }
    );

    m_renderer->update();
    m_window->setStatus(karin::Window::ShowStatus::SHOW);

    Application& app = Application::instance();
    Event event;
    while (app.waitEvent(event)) {}

    m_renderer->cleanUp();
}

void Window::setRootView(std::unique_ptr<ViewNode> rootView)
{
        m_rootView = std::move(rootView);
}
} // karin::gui