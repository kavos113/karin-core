#ifndef SRC_SYSTEM_X11_X11_CONTEXT_H
#define SRC_SYSTEM_X11_X11_CONTEXT_H

#include <X11/Xlib.h>

namespace karin
{
class X11Context
{
public:
    static X11Context& instance();

    Display *display() const;
private:
    X11Context();
    ~X11Context();

    Display *m_display = nullptr;
};
} // karin

#endif //SRC_SYSTEM_X11_X11_CONTEXT_H