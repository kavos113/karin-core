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

void Application::run()
{
    m_impl->run();
}

Window Application::createWindow(const std::wstring& title, int x, int y, int width, int height)
{
    return Window(
        m_impl.get(),
        title,
        x,
        y,
        width,
        height
    );
}

Window Application::createWindow(const std::wstring& title, Rectangle rect)
{
    return Window(
        m_impl.get(),
        title,
        rect
    );
}

Application::Application()
{
    m_impl = createApplicationImpl();
}

SystemFont* Application::systemFont()
{
    return &m_systemFont;
}
}