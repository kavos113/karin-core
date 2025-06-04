#ifndef KARIN_GRAPHICS_RESOURCES_GRAPHICS_DEVICE_H
#define KARIN_GRAPHICS_RESOURCES_GRAPHICS_DEVICE_H
#include <memory>

namespace karin
{

/**
 * GraphicsDevice has the responsibility of managing the graphics hardware resources like:
 * - D2DFactory, D2DDevice, D2DDeviceContext, DWriteFactory, DXGIFactory, etc. (Direct2D, DirectWrite, Direct3D).
 * - VkInstance, VkDevice, VkPhysicalDevice, VkQueue, etc. (Vulkan).
 *
 * usually, it is a singleton that is created at the start of the application and destroyed at the end.
 */
class GraphicsDevice
{
public:
    virtual ~GraphicsDevice() = default;

    virtual void cleanUp() = 0;

    static std::unique_ptr<GraphicsDevice> create();
};

} // karin

#endif //KARIN_GRAPHICS_RESOURCES_GRAPHICS_DEVICE_H
