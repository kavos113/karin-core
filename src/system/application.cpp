#include <karin/system/application.h>

#include "platform.h"
#include "application_impl.h"

namespace karin
{
Application& Application::instance()
{
}

void Application::run()
{
}

Application::Application()
{
    m_impl = createApplicationImpl();
}
}
