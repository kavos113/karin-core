#include <karin/system.h>
#include <karin/graphics.h>
#include <karin/common.h>

#include <memory>
#include <numbers>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window = app.createWindow(L"Hello Graphics", 100, 100, 800, 600);

    std::unique_ptr<karin::GraphicsDevice> device = karin::GraphicsDevice::create();
    karin::Renderer renderer(device.get(), &window, app.systemFont());

    karin::Pattern redPattern = karin::SolidColorPattern(karin::Color(karin::Color::Red));

    karin::Path path;
    path.start(karin::Point(100, 100));
    path.lineTo(karin::Point(150, 100));
    path.arcTo(karin::Point(150, 150), 50.0f, 50.0f, std::numbers::pi / 2, 0.0f, true);
    path.arcTo(karin::Point(250, 150), 50.0f, 50.0f, std::numbers::pi, std::numbers::pi / 2 * 3, true);
    path.lineTo(karin::Point(300, 200));
    path.arcTo(karin::Point(300, 250), 100.0f, 50.0f, std::numbers::pi / 2, std::numbers::pi, false);
    path.arcTo(karin::Point(150, 250), 50.0f, 50.0f, 0.0f, std::numbers::pi / 2, true);
    path.lineTo(karin::Point(100, 200));
    path.close();

    renderer.addDrawCommand(
        [&redPattern, &path](karin::GraphicsContext& gc)
        {
            karin::StrokeStyle strokeStyle = {
                .width = 6.0f,
                .start_cap_style = karin::StrokeStyle::CapStyle::Round,
                .end_cap_style = karin::StrokeStyle::CapStyle::Round,
                .dash_cap_style = karin::StrokeStyle::CapStyle::Triangle,
                .join_style = karin::StrokeStyle::JoinStyle::Round,
                .miter_limit = 10.0f,
                .dash_pattern = {5.0f, 2.0f},
                .dash_offset = 1.0f
            };
            gc.drawPath(path, redPattern, strokeStyle);
        }
    );

    renderer.update();

    window.setStatus(karin::Window::ShowStatus::SHOW);
    app.run();

    renderer.cleanUp();
    device->cleanUp();

    return 0;
}