#include <karin/system.h>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window(L"Hello Window", 100, 100, 800, 600);

    window.setStatus(karin::Window::ShowStatus::SHOW);

    app.run();

    return 0;
}