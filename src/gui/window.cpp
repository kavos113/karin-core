#include "window.h"

#include <karin/system/application.h>

namespace karin::gui
{
Window::Window()
{
    Application& app = Application::instance();
    m_window = app.createWindow(L"Karin GUI Window", 100, 100, 800, 600);
}

void Window::run()
{
}

void Window::setRootView(std::unique_ptr<ViewNode> rootView)
{
    m_rootView = std::move(rootView);
}
} // karin::gui