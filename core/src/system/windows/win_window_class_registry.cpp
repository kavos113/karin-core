#include "win_window_class_registry.h"

#include <iostream>

namespace karin
{
void WinWindowClassRegistry::registerClass(WNDCLASSEX& wc, const std::wstring& className)
{
    wc.lpszClassName = className.c_str();

    if (!RegisterClassEx(&wc))
    {
        std::wcerr << L"Failed to register window class: " << className << std::endl;
        throw std::runtime_error("Failed to register window class");
    }

    m_registeredClasses.push_back(className);
}

void WinWindowClassRegistry::unregisterClasses() const
{
    for (const auto& className : m_registeredClasses)
    {
        if (!UnregisterClass(className.c_str(), GetModuleHandle(nullptr)))
        {
            std::wcerr << L"Failed to unregister window class: " << className << std::endl;
        }
    }
}
} // karin