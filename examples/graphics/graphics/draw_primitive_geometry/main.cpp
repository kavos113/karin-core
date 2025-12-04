#include <karin/system.h>
#include <karin/graphics.h>
#include <karin/common.h>

#include <memory>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window = app.createWindow(L"Hello Graphics", 100, 100, 800, 600);

    std::unique_ptr<karin::GraphicsDevice> device = karin::GraphicsDevice::create();
    karin::Renderer renderer(device.get(), &window, app.systemFont());

    karin::Pattern redPattern = karin::SolidColorPattern(karin::Color(karin::Color::Red));
    karin::Pattern bluePattern = karin::SolidColorPattern(karin::Color(karin::Color::Blue));

    renderer.addDrawCommand(
        [&redPattern, &bluePattern](karin::GraphicsContext& gc)
        {
            gc.drawRect(karin::Rectangle(100, 100, 200, 200), redPattern);
            gc.drawEllipse(karin::Point(400, 300), 100, 50, bluePattern);
            gc.drawRoundedRect(karin::Rectangle(500, 100, 200, 200), 20, 50, bluePattern);

            karin::StrokeStyle strokeStyle = {
                .width = 15.0f,
                .start_cap_style = karin::StrokeStyle::CapStyle::Round,
                .end_cap_style = karin::StrokeStyle::CapStyle::Round,
                .dash_cap_style = karin::StrokeStyle::CapStyle::Triangle,
                .join_style = karin::StrokeStyle::JoinStyle::Round,
                .miter_limit = 10.0f,
                .dash_pattern = {5.0f, 2.0f},
                .dash_offset = 1.0f
            };
            gc.drawLine(karin::Point(100, 400), karin::Point(300, 500), redPattern, strokeStyle);
            gc.drawRect(karin::Rectangle(400, 400, 200, 200), redPattern, strokeStyle);
        }
    );

    renderer.update();

    window.setStatus(karin::Window::ShowStatus::SHOW);

    karin::Event event;
    while (app.waitEvent(event)) {}

    renderer.cleanUp();
    device->cleanUp();

    return 0;
}