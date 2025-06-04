#ifndef SRC_SYSTEM_WINDOWS_WIN_WINDOW_IMPL_H
#define SRC_SYSTEM_WINDOWS_WIN_WINDOW_IMPL_H

#include <windows.h>
#include <window_impl.h>

#include <mutex>
#include <string>
#include <memory>

namespace karin {

class WinWindowImpl : public IWindowImpl
{
public:
    WinWindowImpl(
        const std::wstring &title,
        int x,
        int y,
        int width,
        int height
    );
    ~WinWindowImpl() override = default;

    void show() override;
    void hide() override;
    void minimize() override;
    void maximize() override;

    void setPosition(int x, int y) override;
    void setSize(int width, int height) override;
    void setRect(int x, int y, int width, int height) override;

    [[nodiscard]] void* handle() const override;

private:
    static void registerClass();
    static std::once_flag m_registerClassFlag;

    static LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;

    static constexpr auto CLASS_NAME = L"KarinWindow";
};

} // karin

#endif //SRC_SYSTEM_WINDOWS_WIN_WINDOW_IMPL_H
