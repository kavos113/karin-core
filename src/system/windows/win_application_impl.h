#ifndef SRC_SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H
#define SRC_SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H

#include <application_impl.h>

namespace karin {

class WinApplicationImpl : public IApplicationImpl
{
public:
    WinApplicationImpl();
    ~WinApplicationImpl() override;

    void run() override;
    void shutdown() override;

    bool m_isRunning = false;
};

} // karin

#endif //SRC_SYSTEM_WINDOWS_WIN_APPLICATION_IMPL_H
