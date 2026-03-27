#include "event_dispatcher.h"

#include <karin/common/geometry/point.h>

namespace karin::gui
{
EventDispatcher::EventDispatcher(ViewNode* rootView)
    : m_rootView(rootView)
{
}

void EventDispatcher::dispatchEvent(const Event& event)
{
    if (!m_rootView)
    {
        return;
    }

    std::visit(
        [this]<typename T0>(const T0& e)
        {
            using T = std::decay_t<T0>;
            if constexpr (std::is_same_v<T, MouseMoveEvent>)
            {
                handleMouseMoveEvent(e);
            }
            else if constexpr (std::is_same_v<T, MouseButtonEvent>)
            {
                handleMouseButtonEvent(e);
            }
            else if constexpr (std::is_same_v<T, MouseWheelEvent>)
            {
                handleMouseWheelEvent(e);
            }
        },
        event
    );
}

void EventDispatcher::handleMouseMoveEvent(const MouseMoveEvent& event)
{
}

void EventDispatcher::handleMouseButtonEvent(const MouseButtonEvent& event)
{
    Point point(static_cast<float>(event.x), static_cast<float>(event.y));
    ViewNode *target = m_rootView->hitTest(point);

    switch (event.type)
    {
    case MouseButtonEvent::Type::ButtonPress_:
        pressedNode = target;
        break;

    case MouseButtonEvent::Type::ButtonRelease_:
        if (pressedNode && pressedNode == target)
        {
            pressedNode->triggerClick(point);
        }
        pressedNode = nullptr;
        break;
    }
}

void EventDispatcher::handleMouseWheelEvent(const MouseWheelEvent& event)
{
}
} // karin::gui
