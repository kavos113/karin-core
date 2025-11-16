#include <karin/system.h>

int main()
{
    karin::Application& app = karin::Application::instance();
    karin::Window window = app.createWindow(L"Hello Window", 100, 100, 800, 600);

    window.setStatus(karin::Window::ShowStatus::SHOW);

    app.run();

    /*
     * Event e;
     * while (app.pollEvent(e))
     * {
     *    switch (e.type)
     *    {
     *    case Event::Type::CLOSE:
     *        // Handle close event
     *        break;
     *    case Event::Type::RESIZE:
     *        // Handle resize event
     *        break;
     *    case Event::Type::PAINT:
     *        GraphicsContext gc = renderer.beginFrame();
     *        // Perform drawing operations using gc
     *        renderer.endFrame();
     *        break;
     *    // Handle other event types...
     *    }
     * }
     */

    return 0;
}