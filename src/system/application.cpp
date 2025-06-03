#include "application.h"

#include "platform.h"

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
    impl_ = createApplicationImpl();
}
}
