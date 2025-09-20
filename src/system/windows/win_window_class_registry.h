#ifndef SRC_SYSTEM_WINDOWS_WIN_WINDOW_CLASS_MANAGER_H
#define SRC_SYSTEM_WINDOWS_WIN_WINDOW_CLASS_MANAGER_H

#include <stdexcept>
#include <vector>
#include <windows.h>

namespace karin
{
class WinWindowClassRegistry
{
public:
    static void registerClass(WNDCLASSEX& wc, const std::wstring& className);
    static void unregisterClasses();

private:
    static std::vector<std::wstring> m_registeredClasses;
};
} // karin

#endif //SRC_SYSTEM_WINDOWS_WIN_WINDOW_CLASS_MANAGER_H