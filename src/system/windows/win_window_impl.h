#ifndef SRC_SYSTEM_WINDOWS_WIN_WINDOW_IMPL_H
#define SRC_SYSTEM_WINDOWS_WIN_WINDOW_IMPL_H

#include <windows.h>
#include <window_impl.h>

#include <karin/system/window.h>
#include <string>
#include <functional>

#include "win_application_impl.h"

namespace karin
{
class WinWindowImpl : public IWindowImpl
{
public:
    WinWindowImpl(
        const std::wstring& title,
        int x,
        int y,
        int width,
        int height,
        WinApplicationImpl* appImpl
    );
    ~WinWindowImpl() override = default;

    void show() override;
    void hide() override;
    void minimize() override;
    void maximize() override;

    void setPosition(int x, int y) override;
    void setSize(int width, int height) override;
    void setRect(int x, int y, int width, int height) override;

    void setOnPaint(std::function<bool()> onPaint) override;
    void setOnResize(std::function<void(Size)> onResize) override;

    [[nodiscard]] Window::NativeHandle handle() const override;

    static LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam) const;

    HWND m_hwnd;

    std::function<void()> m_onPaint;
    std::function<void(Size)> m_onResize;

    WinApplicationImpl* m_appImpl = nullptr;
};
} // karin

#endif //SRC_SYSTEM_WINDOWS_WIN_WINDOW_IMPL_H