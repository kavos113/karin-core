#include <karin/system.h>
#include <karin/graphics.h>
#include <karin/common.h>

#include <memory>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window = app.createWindow(L"Hello Graphics", 100, 100, 800, 600);

    karin::Renderer renderer(&window, app.systemFont());
    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::Image image = renderer.createImage("square.png");

    renderer.addDrawCommand(
        [&image](karin::GraphicsContext& gc)
        {
            gc.drawImage(image, karin::Rectangle(100, 100, 200, 200), karin::Rectangle(100, 100, 100, 100));
        }
    );

    renderer.update();

    window.setStatus(karin::Window::ShowStatus::SHOW);

    karin::Event event;
    while (app.waitEvent(event)) {}

    renderer.cleanUp();

    return 0;
}
