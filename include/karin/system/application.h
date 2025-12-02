#ifndef KARIN_SYSTEM_APPLICATION_H
#define KARIN_SYSTEM_APPLICATION_H

#include "system_font.h"
#include "window.h"
#include "event.h"

#include <memory>


namespace karin
{
class IApplicationImpl;

class Application
{
public:
    static Application& instance();

    bool pollEvent(Event& event) const;

    Window createWindow(
        const std::wstring& title,
        int x = 0,
        int y = 0,
        int width = Window::DEFAULT_WIDTH,
        int height = Window::DEFAULT_HEIGHT
    );

    Window createWindow(
        const std::wstring& title,
        Rectangle rect = Rectangle(0, 0, Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT)
    );

    SystemFont* systemFont();

private:
    Application();
    ~Application() = default;

    std::unique_ptr<IApplicationImpl> m_impl;

    SystemFont m_systemFont;
};
} // karin

#endif //KARIN_SYSTEM_APPLICATION_H
