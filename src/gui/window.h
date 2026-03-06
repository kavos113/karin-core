#ifndef SRC_GUI_WINDOW_H
#define SRC_GUI_WINDOW_H

#include <karin/system/window.h>
#include <karin/graphics/renderer.h>

#include "view_node.h"

#include <memory>

namespace karin::gui
{
class Window
{
public:
    Window();
    ~Window() = default;

    void run();
    void setRootView(std::unique_ptr<ViewNode> rootView);

private:
    std::unique_ptr<ViewNode> m_rootView;

    std::unique_ptr<karin::Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
};
} // karin::gui

#endif //SRC_GUI_WINDOW_H