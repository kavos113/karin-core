#include <karin/gui/window.h>

#include <karin/system/application.h>
#include <karin/common/color/color.h>

#include "event_dispatcher.h"

namespace karin::gui
{
Window::Window(const std::string& title, int x, int y, int width, int height)
{
    karin::Application& app = karin::Application::instance();
    m_window = app.createWindow(title, x, y, width, height);
    m_window->setUserData(this);

    m_renderer = std::make_unique<Renderer>(m_window.get());
    m_renderer->setClearColor(Color(Color::White));

    m_eventDispatcher = std::make_unique<EventDispatcher>(m_rootView.get());
}

Window::~Window()
{
    m_renderer->cleanUp();
}

void Window::beforeRun()
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
            m_rootView->setSize(size);
        }
    );

    m_renderer->update();
    m_window->setStatus(karin::Window::ShowStatus::SHOW);
}

void Window::dispatchEvent(const Event& event) const
{
    if (m_eventDispatcher)
    {
        m_eventDispatcher->dispatchEvent(event);
    }
}

void Window::setRootView(std::unique_ptr<ViewNode> rootView)
{
    m_rootView = std::move(rootView);
    m_eventDispatcher = std::make_unique<EventDispatcher>(m_rootView.get());
}

void Window::requestRelayout()
{
    m_needRelayout = true;
    m_window->invalidate();
}
} // karin::gui