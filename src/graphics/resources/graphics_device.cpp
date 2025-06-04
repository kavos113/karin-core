#include <karin/graphics/resources/graphics_device.h>

#include "graphics_device_impl.h"
#include "platform.h"

namespace karin
{
GraphicsDevice::GraphicsDevice()
    : m_impl(createGraphicsDeviceImpl())
{
}

void GraphicsDevice::cleanUp()
{
    m_impl->cleanUp();
}

} // karin