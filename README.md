# karin-core
cross-platform GUI library with Direct2D(Windows) and Vulkan(UNIX) backends

## development

### dependencies
Window
- libx11(UNIX)
Graphics
- Vulkan SDK(UNIX)
- vcpkg
  - glm(UNIX)
  - Vulkan Memory Allocator(UNIX)

### installation

```shell
vcpkg install
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake
cmake --build build
```