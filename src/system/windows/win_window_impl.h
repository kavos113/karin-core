#ifndef SRC_SYSTEM_WINDOWS_WIN_WINDOW_IMPL_H
#define SRC_SYSTEM_WINDOWS_WIN_WINDOW_IMPL_H

#include <windows.h>
#include <window_impl.h>

#include <karin/system/window.h>
#include <string>
#include <functional>
#include <vector>

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

    void addPaintCallback(std::function<bool()> onPaint) override;
    void addResizeCallback(std::function<void(Size)> onResize) override;
    void addStartResizeCallback(std::function<void()> onStartResize) override;
    void addFinishResizeCallback(std::function<void()> onFinishResize) override;

    [[nodiscard]] Window::NativeHandle handle() const override;

    static LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam) const;

    HWND m_hwnd;

    std::vector<std::function<bool()>> m_paintCallbacks;
    std::vector<std::function<void(Size)>> m_resizeCallbacks;
    std::vector<std::function<void()>> m_startResizeCallbacks;
    std::vector<std::function<void()>> m_finishResizeCallbacks;

    WinApplicationImpl* m_appImpl = nullptr;
};
} // karin

#endif //SRC_SYSTEM_WINDOWS_WIN_WINDOW_IMPL_H