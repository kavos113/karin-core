#include <karin/system.h>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window = app.createWindow(L"Hello Window", 100, 100, 800, 600);

    window.setStatus(karin::Window::ShowStatus::SHOW);

    window.setOnResize([](karin::Size newSize)
    {
        std::cout << "Window resized to " << newSize.width << "x" << newSize.height << std::endl;
    });

    karin::Event event;
    while (app.waitEvent(event))
    {
        if (const auto* windowEvent = std::get_if<karin::WindowEvent>(&event))
        {
            if (windowEvent->type == karin::WindowEvent::Type::Close)
            {
                std::cout << "Window close event received." << std::endl;
            }
        }
    }

    return 0;
}