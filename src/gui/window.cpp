#include "window.h"

#include <karin/system/application.h>
#include <karin/common/color/color.h>

namespace karin::gui
{
Window::Window(const std::wstring& title, int x, int y, int width, int height)
{
    Application& app = Application::instance();
    m_window = app.createWindow(title, x, y, width, height);

    m_renderer = std::make_unique<Renderer>(m_window.get());
    m_renderer->setClearColor(Color(Color::White));
}

void Window::run()
{
    m_rootView->calculateLayout();
    m_needRelayout = false;

    m_renderer->addDrawCommand(
        [this](GraphicsContext& gc)
        {
            if (m_needRelayout)
            {
                m_rootView->calculateLayout();
                m_needRelayout = false;
            }

            Rectangle layout = m_rootView->getLayout();
            Transform2D transform;
            transform.translate(layout.pos.x, layout.pos.y);

            m_rootView->draw(gc, transform);
        }
    );

    m_window->addResizeCallback(
        [this](Size size)
        {
            if (size.width == 0 || size.height == 0)
            {
                return;
            }
            requestRelayout();
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

void Window::requestRelayout()
{
    m_needRelayout = true;
    m_window->invalidate();
}
} // karin::gui