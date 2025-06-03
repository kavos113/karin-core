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

Application::Application()
{
    m_impl = createApplicationImpl();
}
}
