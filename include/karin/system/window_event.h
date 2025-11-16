#ifndef KARIN_SYSTEM_WINDOW_EVENT_H
#define KARIN_SYSTEM_WINDOW_EVENT_H

#include "event.h"

namespace karin
{
class CloseEvent : public Event
{
public:
    CloseEvent()
        : Event(Type::WindowClose)
    {
    }
};

class MaximizeEvent : public Event
{
public:
    MaximizeEvent()
        : Event(Type::WindowMaximize)
    {
    }
};

class MinimizeEvent : public Event
{
public:
    MinimizeEvent()
        : Event(Type::WindowMinimize)
    {
    }
};

class ResizeEvent : public Event
{
public:
    ResizeEvent(int width, int height)
        : Event(Type::WindowResize)
        , m_width(width)
        , m_height(height)
    {
    }

private:
    int m_width;
    int m_height;
};

class PaintEvent : public Event
{
public:
    PaintEvent()
        : Event(Type::WindowPaint)
    {
    }
};
} // karin

#endif //KARIN_SYSTEM_WINDOW_EVENT_H