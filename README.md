# karin-core
cross-platform GUI library with Direct2D and Vulkan backends

## development

### dependencies
- Vulkan SDK(when using Vulkan backend)
- Windows SDK(when using Direct2D backend)

### installation
```shell
vcpkg install
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

if you want to use Vulkan backend on Windows, add `-DWINDOWS_VULKAN=ON` to the cmake command:
```shell
vcpkg install
cmake -B build -S . -DWINDOWS_VULKAN=ON -DCMAKE_TOOLCHAIN_FILE=<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake
cmake --build build
```