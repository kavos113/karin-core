#include <karin/system.h>
#include <karin/common.h>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window(L"Hello Window", karin::Rectangle(100, 100, 800, 600));

    window.setStatus(karin::Window::ShowStatus::SHOW);
    window.setSize(karin::Size(1600, 900));

    app.run();

    return 0;
}