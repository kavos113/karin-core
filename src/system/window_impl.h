#ifndef SRC_SYSTEM_WINDOW_IMPL_H
#define SRC_SYSTEM_WINDOW_IMPL_H
#include <functional>

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

    virtual void setOnPaint(std::function<void()> onPaint) = 0;

    virtual void* handle() const = 0;
};

} // karin

#endif //SRC_SYSTEM_WINDOW_IMPL_H
