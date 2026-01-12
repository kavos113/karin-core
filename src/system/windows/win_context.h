#ifndef SRC_SYSTEM_WINDOWS_WIN_CONTEXT_H
#define SRC_SYSTEM_WINDOWS_WIN_CONTEXT_H
#include "win_window_class_registry.h"

namespace karin
{
// no contents yet
class WinContext
{
public:
    static WinContext& instance();

    void init();
    void cleanup();

    WinWindowClassRegistry& windowClassRegistry()
    {
        return m_windowsClassRegistry;
    }

private:
    WinContext() = default;
    ~WinContext() = default;

    WinWindowClassRegistry m_windowsClassRegistry;
};
} // karin

#endif //SRC_SYSTEM_WINDOWS_WIN_CONTEXT_H