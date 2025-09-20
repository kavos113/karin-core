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
            gc.fillRect(karin::Rectangle(100, 100, 200, 200), redPattern);
            gc.fillEllipse(karin::Point(400, 300), 100, 50, bluePattern);
            gc.fillRoundedRect(karin::Rectangle(500, 100, 200, 200), 20, 50, redPattern);
        }
    );

    renderer.update();

    window.setStatus(karin::Window::ShowStatus::SHOW);
    app.run();

    renderer.cleanUp();
    device->cleanUp();

    return 0;
}