#include <karin/graphics/resources/graphics_device.h>

#include "resources/platform.h"

namespace karin
{

std::unique_ptr<GraphicsDevice> GraphicsDevice::create()
{
    return createGraphicsDevice();
}

}
