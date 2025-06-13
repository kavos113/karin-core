#ifndef SRC_SYSTEM_WINDOW_IMPL_H
#define SRC_SYSTEM_WINDOW_IMPL_H
#include <functional>

#include <karin/common/geometry/size.h>
#include <karin/system/window.h>

namespace karin {

class IWindowImpl
{
public:
    virtual ~IWindowImpl() = default;

    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void minimize() = 0;
    virtual void maximize() = 0;

    virtual void setPosition(int x, int y) = 0;
    virtual void setSize(int width, int height) = 0;
    virtual void setRect(int x, int y, int width, int height) = 0;

    virtual void setOnPaint(std::function<bool()> onPaint) = 0;
    virtual void setOnResize(std::function<void(Size)> onResize) = 0;

    virtual Window::NativeHandle handle() const = 0;
};

} // karin

#endif //SRC_SYSTEM_WINDOW_IMPL_H
