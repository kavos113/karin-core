#include "win_context.h"

namespace karin
{

WinContext& WinContext::instance()
{
    static WinContext instance;
    return instance;
}

WinContext::WinContext() = default;

WinContext::~WinContext()
{
    m_windowsClassRegistry.unregisterClasses();
}
} // karin