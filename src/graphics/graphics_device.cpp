#include <karin/graphics/graphics_device.h>

#include "platform.h"

namespace karin
{

std::unique_ptr<GraphicsDevice> GraphicsDevice::create()
{
    return createGraphicsDevice();
}

}
