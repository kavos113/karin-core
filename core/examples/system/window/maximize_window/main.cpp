#include <karin/system.h>

int main()
{
    karin::Application& app = karin::Application::instance();
    std::unique_ptr<karin::Window> window = app.createWindow("Maximize Window Example", 100, 100, 800, 600);

    window->setStatus(karin::Window::ShowStatus::MAXIMIZE);

    karin::EventPayload event;
    while (app.waitEvent(event)) {}

    return 0;
}