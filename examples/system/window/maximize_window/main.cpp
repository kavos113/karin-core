#include <karin/system.h>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window = app.createWindow(L"Maximize Window Example", 100, 100, 800, 600);

    window.setStatus(karin::Window::ShowStatus::MAXIMIZE);

    karin::Event event;
    while (app.waitEvent(event)) {}

    return 0;
}