#ifndef SRC_SYSTEM_WINDOWS_WIN_CONTEXT_H
#define SRC_SYSTEM_WINDOWS_WIN_CONTEXT_H
#include "win_window_class_registry.h"

namespace karin
{
class WinContext
{
public:
    static WinContext& instance();

    const WinWindowClassRegistry& windowClassRegistry() const
    {
        return m_windowsClassRegistry;
    }

private:
    WinContext();
    ~WinContext();

    WinWindowClassRegistry m_windowsClassRegistry;
};
} // karin

#endif //SRC_SYSTEM_WINDOWS_WIN_CONTEXT_H