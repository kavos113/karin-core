#ifndef SRC_GUI_WINDOW_H
#define SRC_GUI_WINDOW_H

#include <karin/system/window.h>
#include <karin/system/event.h>
#include <karin/graphics/renderer.h>

#include "view_node.h"

#include <memory>
#include <string>

namespace karin::gui
{
class Application;
class EventDispatcher;

class Window
{
private:
    friend class Application;

    void beforeRun();

public:
    Window(
        const std::string& title,
        int x,
        int y,
        int width,
        int height
    );
    ~Window();

    void setRootView(std::unique_ptr<ViewNode> rootView);

    void dispatchEvent(const Event& event);

private:
    void requestRelayout();

    std::unique_ptr<ViewNode> m_rootView;

    std::unique_ptr<karin::Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<EventDispatcher> m_eventDispatcher;

    bool m_needRelayout = true;
};
} // karin::gui

#endif //SRC_GUI_WINDOW_H!