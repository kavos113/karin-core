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
    void registerClass(WNDCLASSEX& wc, const std::wstring& className);
    void unregisterClasses() const;

private:
    std::vector<std::wstring> m_registeredClasses;
};
} // karin

#endif //SRC_SYSTEM_WINDOWS_WIN_WINDOW_CLASS_MANAGER_H