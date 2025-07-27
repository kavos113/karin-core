#include <karin/system.h>
#include <karin/graphics.h>
#include <karin/common.h>

#include <memory>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window = app.createWindow(L"Hello Graphics", 100, 100, 800, 600);

    std::unique_ptr<karin::GraphicsDevice> device = karin::GraphicsDevice::create();
    karin::Renderer renderer(device.get(), &window);

    auto redPattern = std::make_unique<karin::SolidColorPattern>(karin::Color(karin::Color::Red));
    auto bluePattern = std::make_unique<karin::SolidColorPattern>(karin::Color(karin::Color::Blue));

    karin::Path path;
    path.start(karin::Point(100, 100));
    path.lineTo(karin::Point(300, 100));
    path.arcTo(karin::Point(300, 200), 50, 50, 0.0f, 180.0f);
    path.lineTo(karin::Point(100, 200));
    path.arcTo(karin::Point(100, 100), 50, 50, 180.0f, 360.0f);
    path.close();

    renderer.addDrawCommand([&redPattern, &path](karin::GraphicsContext& gc)
    {
        karin::StrokeStyle strokeStyle = {
            .width = 15.0f,
            .start_cap_style = karin::StrokeStyle::CapStyle::Round,
            .end_cap_style = karin::StrokeStyle::CapStyle::Round,
            .dash_cap_style = karin::StrokeStyle::CapStyle::Triangle,
            .join_style = karin::StrokeStyle::JoinStyle::Round,
            .miter_limit = 10.0f,
            .dash_pattern = { 5.0f, 2.0f },
            .dash_offset = 1.0f
        };
        gc.drawPath(path, redPattern.get(), strokeStyle);
    });

    renderer.update();

    window.setStatus(karin::Window::ShowStatus::SHOW);
    app.run();

    renderer.cleanUp();
    device->cleanUp();

    return 0;
}
