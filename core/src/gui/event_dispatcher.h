#ifndef CORE_SRC_GUI_EVENT_DISPATCHER_H
#define CORE_SRC_GUI_EVENT_DISPATCHER_H

#include <karin/system/event.h>
#include <karin/gui/view_node.h>

namespace karin::gui
{

class EventDispatcher
{
public:
    explicit EventDispatcher(ViewNode *rootView);

    void dispatchEvent(const Event& event);

private:
    void handleMouseMoveEvent(const MouseMoveEvent& event);
    void handleMouseButtonEvent(const MouseButtonEvent& event);
    void handleMouseWheelEvent(const MouseWheelEvent& event);

    ViewNode *m_rootView;

    ViewNode *m_pressedNode = nullptr;
    ViewNode *m_hoveredNode = nullptr;
    ViewNode *m_focusedNode = nullptr;
};

} // karin::gui

#endif //CORE_SRC_GUI_EVENT_DISPATCHER_H
