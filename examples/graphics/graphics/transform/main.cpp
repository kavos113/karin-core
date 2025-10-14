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

    renderer.setClearColor(karin::Color(karin::Color::Green));

    karin::Pattern cyanPattern = karin::SolidColorPattern(karin::Color(0, 255, 255));
    karin::Pattern magentaPattern = karin::SolidColorPattern(karin::Color(255, 0, 255));
    karin::Pattern yellowPattern = karin::SolidColorPattern(karin::Color(255, 255, 0));
    karin::Pattern blackPattern = karin::SolidColorPattern(karin::Color(0, 0, 0));
    karin::Pattern whitePattern = karin::SolidColorPattern(karin::Color(255, 255, 255));

    renderer.addDrawCommand(
        [&cyanPattern, &magentaPattern, &yellowPattern, &blackPattern, &whitePattern](karin::GraphicsContext& gc)
        {
            gc.fillRect(
                karin::Rectangle(100, 100, 100, 100),
                cyanPattern,
                karin::Transform2D()
                    .rotate(-std::numbers::pi / 4)
                    .translate(0, 100)
            );

            gc.fillEllipse(
                karin::Point(400, 300), 50, 100,
                magentaPattern,
                karin::Transform2D()
                    .rotate(std::numbers::pi / 4)
                    .translate(100, 0)
            );

            gc.drawLine(
                karin::Point(600, 100), karin::Point(700, 200),
                blackPattern,
                karin::StrokeStyle(5),
                karin::Transform2D()
                    .rotate(std::numbers::pi / 6)
                    .translate(-50, 0)
            );

            gc.drawRect(
                karin::Rectangle(500, 400, 150, 100),
                whitePattern,
                karin::StrokeStyle(3),
                karin::Transform2D()
                    .rotate(-std::numbers::pi / 3)
                    .translate(0, -50)
            );

            gc.drawRoundedRect(
                karin::Rectangle(200, 400, 150, 100),
                20, 20,
                yellowPattern,
                karin::StrokeStyle(4),
                karin::Transform2D()
                    .rotate(std::numbers::pi / 8)
                    .translate(0, -50)
            );
        }
    );

    renderer.update();

    window.setStatus(karin::Window::ShowStatus::SHOW);
    app.run();

    renderer.cleanUp();
    device->cleanUp();

    return 0;
}
