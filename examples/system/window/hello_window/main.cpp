#include <karin/system.h>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window = app.createWindow(L"Hello Window", 100, 100, 800, 600);

    window.setStatus(karin::Window::ShowStatus::SHOW);

    karin::Event event;
    while (app.pollEvent(event))
    {
        if (const auto* windowEvent = std::get_if<karin::WindowEvent>(&event))
        {
            if (windowEvent->type == karin::WindowEvent::Type::Close)
            {
                std::cout << "Window close event received." << std::endl;
            }
        }
        else if (const auto* resizeEvent = std::get_if<karin::WindowResizeEvent>(&event))
        {
            int newWidth = resizeEvent->width;
            int newHeight = resizeEvent->height;

            std::cout << "Window resized to " << newWidth << "x" << newHeight << std::endl;
        }
    }

    return 0;
}