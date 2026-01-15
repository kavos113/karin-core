#ifndef SRC_SYSTEM_X11_X11_WINDOW_IMPL_H
#define SRC_SYSTEM_X11_X11_WINDOW_IMPL_H
#include <mutex>
#include <string>
#include <optional>
#include <X11/Xlib.h>

#include <karin/system/window.h>
#include <karin/system/event.h>
#include <x11/window.h>
#include <window_impl.h>
#include "x11_application_impl.h"

namespace karin
{
class X11WindowImpl : public IWindowImpl
{
public:
    X11WindowImpl(
        const std::wstring& title,
        int x,
        int y,
        int width,
        int height,
        X11ApplicationImpl* appImpl
    );
    ~X11WindowImpl() override;

    void show() override;
    void hide() override;
    void minimize() override;
    void maximize() override;

    void setPosition(int x, int y) override;
    void setSize(int width, int height) override;
    void setRect(int x, int y, int width, int height) override;

    void setOnPaint(std::function<bool()> onPaint) override;
    void setOnResize(std::function<void(Size)> onResize) override;
    void setOnStartResize(std::function<void()> onStartResize) override;
    void setOnFinishResize(std::function<void()> onFinishResize) override;

    [[nodiscard]] Window::NativeHandle handle() const override;

    void handleEvent(const XEvent& event);

private:
    void applyStatus();
    std::optional<Event> translateX11Event(XEvent* event);

    XlibWindow m_window;
    GC m_gc;
    XIM m_xim;
    XIC m_xic;

    std::function<bool()> m_onPaint;
    std::function<void(Size)> m_onResize;
    std::function<void()> m_onStartResize;
    std::function<void()> m_onFinishResize;

    std::function<void()> m_onClose;
    std::function<void()> m_onStartMainLoop;

    enum class X11ShowStatus
    {
        Maximize,
        Minimize,
        NoStatus
    };

    X11ShowStatus m_status = X11ShowStatus::NoStatus;
    std::once_flag m_applyStatusFlag;

    X11ApplicationImpl* m_appImpl = nullptr;

    static constexpr float SCROLL_DELTA = 1.0f;
};
} // karin

#endif //SRC_SYSTEM_X11_X11_WINDOW_IMPL_H