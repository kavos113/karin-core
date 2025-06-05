#include <karin/system.h>
#include <karin/graphics.h>
#include <karin/common.h>

#include <memory>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window(L"Hello Window", 100, 100, 800, 600);

    std::unique_ptr<karin::GraphicsDevice> device = karin::GraphicsDevice::create();
    karin::Renderer renderer(device.get(), &window);

    auto cyanPattern = std::make_unique<karin::SolidColorPattern>(karin::Color(0, 255, 255, 255));

    renderer.addDrawCommand([&cyanPattern](karin::GraphicsContext& gc)
    {
        gc.fillRect(karin::Rectangle(100, 100, 200, 200), cyanPattern.get());
    });

    renderer.update();

    window.setStatus(karin::Window::ShowStatus::SHOW);
    app.run();

    renderer.cleanUp();
    device->cleanUp();

    return 0;
}
