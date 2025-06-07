#ifndef KARIN_SYSTEM_APPLICATION_H
#define KARIN_SYSTEM_APPLICATION_H
#include <memory>

#include "window.h"

namespace karin
{

class IApplicationImpl;

class Application
{
public:
    static Application& instance();

    void run();

    Window createWindow(
        const std::wstring &title,
        int x = 0,
        int y = 0,
        int width = Window::DEFAULT_WIDTH,
        int height = Window::DEFAULT_HEIGHT
    );

    Window createWindow(
        const std::wstring &title,
        Rectangle rect = Rectangle(0, 0, Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT)
    );

private:
    Application();
    ~Application() = default;

    std::unique_ptr<IApplicationImpl> m_impl;
};

} // karin

#endif //KARIN_SYSTEM_APPLICATION_H
