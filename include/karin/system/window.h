#ifndef KARIN_SYSTEM_WINDOW_H
#define KARIN_SYSTEM_WINDOW_H

#include <functional>
#include <memory>
#include <string>

#include "karin/common/rectangle.h"

namespace karin
{

class IWindowImpl;

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

    explicit Window(
        const std::wstring &title,
        int x = 0,
        int y = 0,
        int width = DEFAULT_WIDTH,
        int height = DEFAULT_HEIGHT
    );
    Window(
        const std::wstring &title,
        Rectangle rect = Rectangle(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT)
    );
    ~Window();

    void setStatus(ShowStatus status);
    ShowStatus status() const;

    void* handle() const;

    void setPosition(Point pos);
    void setSize(Size size);
    void setRect(Rectangle rect);
    Point position() const;
    Size size() const;
    Rectangle rect() const;

    void setOnPaint(std::function<void()> onPaint);
    void setOnResize(std::function<void(Size)> onResize);

private:
    ShowStatus m_showStatus = ShowStatus::HIDE;
    Rectangle m_rect;

    std::unique_ptr<IWindowImpl> m_impl;

    static constexpr int DEFAULT_WIDTH = 800;
    static constexpr int DEFAULT_HEIGHT = 600;
};

} // karin

#endif //KARIN_SYSTEM_WINDOW_H
