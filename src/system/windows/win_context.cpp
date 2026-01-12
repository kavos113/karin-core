#include "win_context.h"

namespace karin
{

WinContext& WinContext::instance()
{
    static WinContext instance;
    return instance;
}

void WinContext::init()
{
}

void WinContext::cleanup()
{
    m_windowsClassRegistry.unregisterClasses();
}

WinContext::WinContext() = default;

WinContext::~WinContext() = default;
} // karin