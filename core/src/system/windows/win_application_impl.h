#ifndef SRC_SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H
#define SRC_SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H

#include <application_impl.h>

#include <karin/system/event.h>
#include <karin/system/window.h>
#include <queue>

namespace karin
{
class WinApplicationImpl : public IApplicationImpl
{
public:
    WinApplicationImpl();
    ~WinApplicationImpl() override = default;

    bool waitEvent(EventPayload& event) override;
    void shutdown() override;

    void pushEvent(const Event& event, Window *window)
    {
        EventPayload payload{
            .window = window,
            .event = event
        };
        m_eventQueue.push(payload);
    }

    bool m_isRunning = false;

    static constexpr auto CLASS_NAME = L"KarinWindow";

private:
    std::queue<EventPayload> m_eventQueue;
};
} // karin

#endif //SRC_SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H