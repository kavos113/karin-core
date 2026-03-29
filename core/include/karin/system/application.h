#ifndef KARIN_SYSTEM_APPLICATION_H
#define KARIN_SYSTEM_APPLICATION_H

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

    // return true if event loop should continue, false to quit.
    bool waitEvent(EventPayload& event) const;

    // TODO: peekEvent

    std::unique_ptr<Window> createWindow(
        const std::string& title,
        int x = 0,
        int y = 0,
        int width = Window::DEFAULT_WIDTH,
        int height = Window::DEFAULT_HEIGHT
    );

    std::unique_ptr<Window> createWindow(
        const std::string& title,
        Rectangle rect = Rectangle(0, 0, Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT)
    );

    WindowID registerWindow(Window* window);
    void unregisterWindow(WindowID id);
    Window* findWindow(WindowID id) const;

private:
    Application();
    ~Application() = default;

    std::unique_ptr<IApplicationImpl> m_impl;

    std::unordered_map<WindowID, Window*> m_windowRegistry;
    WindowID m_nextWindowID = 1;
};
} // karin

#endif //KARIN_SYSTEM_APPLICATION_H
