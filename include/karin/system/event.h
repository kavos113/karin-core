#ifndef KARIN_SYSTEM_EVENT_H
#define KARIN_SYSTEM_EVENT_H

namespace karin
{
class Event
{
public:
    enum class Type
    {
        WindowClose,
        WindowResize,
        WindowPaint,
        WindowMaximize,
        WindowMinimize,
        KeyPress,
        KeyRelease,
        MouseMove,
        MouseButtonPress,
        MouseButtonRelease,
        MouseWheel,
    };

    virtual ~Event() = default;
    Type type() const
    {
        return m_type;
    }

protected:
    Event(Type type)
        : m_type(type)
    {
    }

    Type m_type;

};

} // karin

#endif //KARIN_SYSTEM_EVENT_H