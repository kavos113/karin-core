#ifndef KARIN_SYSTEM_MOUSE_EVENT_H
#define KARIN_SYSTEM_MOUSE_EVENT_H

#include "event.h"

namespace karin
{
class MouseMoveEvent : public Event
{
public:
    MouseMoveEvent(int x, int y)
        : Event(Type::MouseMove)
        , m_x(x)
        , m_y(y)
    {
    }

private:
    int m_x;
    int m_y;

};

class MouseButtonEvent : public Event
{
public:
    enum class Button
    {
        LEFT,
        RIGHT,
        MIDDLE
    };

    MouseButtonEvent(Type type, Button button, int x, int y)
        : Event(type)
        , m_button(button)
        , m_x(x)
        , m_y(y)
    {
    }

private:
    Button m_button;
    int m_x;
    int m_y;
};

class MouseWheelEvent : public Event
{
public:
    MouseWheelEvent(int delta, int x, int y)
        : Event(Type::MouseWheel)
        , m_delta(delta)
        , m_x(x)
        , m_y(y)
    {
    }

private:
    int m_delta;
    int m_x;
    int m_y;
};
} // karin

#endif //KARIN_SYSTEM_MOUSE_EVENT_H