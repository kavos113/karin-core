#ifndef SRC_SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H
#define SRC_SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H

#include <application_impl.h>

#include <karin/system/event.h>
#include <queue>

namespace karin
{
class WinApplicationImpl : public IApplicationImpl
{
public:
    WinApplicationImpl();
    ~WinApplicationImpl() override;

    void run() override;
    bool pollEvent(Event& event) override;
    void shutdown() override;

    bool m_isRunning = false;

private:
    std::queue<Event> m_eventQueue;
};
} // karin

#endif //SRC_SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H