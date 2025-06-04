#include <karin/graphics/resources/graphics_device.h>

#include "graphics_device_impl.h"

namespace karin
{

void GraphicsDevice::cleanUp()
{
    m_impl->cleanUp();
}

} // karin