#ifndef KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
#define KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
#include "karin/common/rectangle.h"

namespace karin {

class GraphicsContext
{
public:
    void drawRect(Rectangle rect);
};

} // karin

#endif //KARIN_GRAPHICS_GRAPHICS_GRAPHICS_CONTEXT_H
