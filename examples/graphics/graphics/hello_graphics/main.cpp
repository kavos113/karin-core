#include <karin/system.h>
#include <karin/graphics.h>

#include <memory>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window(L"Hello Window", 100, 100, 800, 600);

    std::unique_ptr<karin::GraphicsDevice> device = karin::GraphicsDevice::create();
    karin::Renderer renderer(device.get(), &window);

    renderer.update();

    window.setStatus(karin::Window::ShowStatus::SHOW);
    app.run();

    renderer.cleanUp();
    device->cleanUp();

    return 0;
}
