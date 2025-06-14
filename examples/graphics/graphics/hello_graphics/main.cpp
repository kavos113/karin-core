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
    renderer.setClearColor(karin::Color(karin::Color::Green));

    auto cyanPattern = std::make_unique<karin::SolidColorPattern>(karin::Color(0, 255, 255, 255));
    auto magentaPattern = std::make_unique<karin::SolidColorPattern>(karin::Color(255, 0, 255, 255));

    renderer.addDrawCommand([&cyanPattern, &magentaPattern](karin::GraphicsContext& gc)
    {
        gc.fillRect(karin::Rectangle(100, 100, 200, 200), cyanPattern.get());
        gc.fillRect(karin::Rectangle(400, 100, 200, 200), magentaPattern.get());
    });

    renderer.update();

    window.setStatus(karin::Window::ShowStatus::SHOW);
    app.run();

    renderer.cleanUp();
    device->cleanUp();

    return 0;
}
