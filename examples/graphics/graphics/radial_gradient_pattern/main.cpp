#include <karin/system.h>
#include <karin/graphics.h>
#include <karin/common.h>

#include <memory>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window = app.createWindow(L"Hello Graphics", 100, 100, 800, 600);

    karin::Renderer renderer(&window, app.systemFont());

    karin::Pattern pattern = karin::RadialGradientPattern{
        .center = karin::Point(400, 300),
        .offset = karin::Point(100, 100),
        .radiusX = 300.0f,
        .radiusY = 200.0f,
        .gradientPoints = {
            .points = {
                {0.0f, karin::Color(karin::Color::Red)},
                {0.5f, karin::Color(karin::Color::Green)},
                {1.0f, karin::Color(karin::Color::Blue)}
            },
            .extendMode = karin::ExtendMode::REPEAT
        },
    };

    renderer.addDrawCommand(
        [&pattern](karin::GraphicsContext& gc)
        {
            gc.fillRect(karin::Rectangle(50, 50, 600, 400), pattern);
        }
    );

    renderer.update();

    window.setStatus(karin::Window::ShowStatus::SHOW);

    karin::Event event;
    while (app.waitEvent(event)) {}

    renderer.cleanUp();

    return 0;
}