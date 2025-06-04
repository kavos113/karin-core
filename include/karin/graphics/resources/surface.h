#ifndef KARIN_GRAPHICS_RESOURCES_SURFACE_H
#define KARIN_GRAPHICS_RESOURCES_SURFACE_H

#include <memory>

namespace karin
{

class ISurfaceImpl;

/**
 * Surface manages swap chain and rendering targets.
 * It is not responsible for the actual rendering(render passes, shaders, etc.).
 */
class surface
{
private:
    std::unique_ptr<ISurfaceImpl> m_impl;
};

} // karin

#endif //KARIN_GRAPHICS_RESOURCES_SURFACE_H
