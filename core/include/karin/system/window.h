#ifndef KARIN_SYSTEM_WINDOW_H
#define KARIN_SYSTEM_WINDOW_H

#include <functional>
#include <memory>
#include <string>

#include "../common/geometry/rectangle.h"

namespace karin
{

class IWindowImpl;
class IApplicationImpl;

class Window
{
public:
    enum class ShowStatus : uint8_t
    {
        HIDE,
        SHOW,
        MINIMIZE,
        MAXIMIZE
    };

    struct NativeHandle
    {
        // WIN32
        void* hwnd = nullptr;
        void* hinstance = nullptr;

        // X11
        void *display = nullptr;
        uint64_t window = 0;
    };

    explicit Window(
        IApplicationImpl *applicationImpl,
        const std::string &title,
        int x = 0,
        int y = 0,
        int width = DEFAULT_WIDTH,
        int height = DEFAULT_HEIGHT
    );
    explicit Window(
        IApplicationImpl *applicationImpl,
        const std::string &title,
        Rectangle rect = Rectangle(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT)
    );
    ~Window();

    void setStatus(ShowStatus status);
    ShowStatus status() const;

    NativeHandle handle() const;

    void setPosition(Point pos);
    void setSize(Size size);
    void setRect(Rectangle rect);
    Point position() const;
    Size size() const;
    Rectangle rect() const;

    // expected to return false if it should redraw
    void addPaintCallback(std::function<bool()> onPaint);
    void addResizeCallback(std::function<void(Size)> onResize);
    void addStartResizeCallback(std::function<void()> onStartResize);
    void addFinishResizeCallback(std::function<void()> onFinishResize);

    // request redraw, will trigger paint callbacks
    void invalidate();

    void setUserData(void* data);
    void* userData() const;

    static constexpr int DEFAULT_WIDTH = 800;
    static constexpr int DEFAULT_HEIGHT = 600;

private:
    ShowStatus m_showStatus = ShowStatus::HIDE;
    Rectangle m_rect;

    void *m_userData = nullptr;

    std::unique_ptr<IWindowImpl> m_impl;
};

} // karin

#endif //KARIN_SYSTEM_WINDOW_H
