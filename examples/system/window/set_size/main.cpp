#include <karin/system.h>
#include <karin/common.h>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window = app.createWindow(L"Set Size Example", 100, 100, 800, 600);

    window.setStatus(karin::Window::ShowStatus::SHOW);
    window.setSize(karin::Size(1600, 900));

    karin::Event event;
    while (app.waitEvent(event)) {}

    return 0;
}