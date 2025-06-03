#include "win_window_class_manager.h"

#include <iostream>

namespace karin
{

void WinWindowClassManager::registerClass(WNDCLASSEX &wc, const std::wstring &className)
{
    wc.lpszClassName = className.c_str();

    if (!RegisterClassEx(&wc))
    {
        throw std::runtime_error("Failed to register window class: " + std::string(className.begin(), className.end()));
    }

    m_registeredClasses.push_back(className);
}

void WinWindowClassManager::unregisterClasses()
{
    for (const auto &className : m_registeredClasses)
    {
        if (!UnregisterClass(className.c_str(), GetModuleHandle(nullptr)))
        {
            std::cerr << "Failed to unregister window class: " << std::string(className.begin(), className.end()) << std::endl;
        }
    }
}

} // karin