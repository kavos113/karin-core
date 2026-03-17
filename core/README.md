# karin-core
cross-platform GUI library with Direct2D and Vulkan backends

## development

### dependencies
- Vulkan SDK(when using Vulkan backend)
- Windows SDK(when using Direct2D backend)
- vcpkg
- cmake

install ubuntu dependencies:
```shell
sudo apt install clang clang-tools libc++-dev libc++abi-dev libx11-dev
```

### installation
cmake presets:
- windows-debug
  - for Windows with Direct2D backend
- windows-vulkan-debug
  - for Windows with Vulkan backend
- ubuntu-debug
  - for Ubuntu with Vulkan backend

It is required to add vcpkg to CMake toolchain file.

example of `CMakeUserPreset.json` (ubuntu):
```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "ubuntu-local-debug",
      "inherits": ["ubuntu-debug"],
      "description": "ubuntu local debug build",
      "cacheVariables": {
        "CMAKE_TOOLCHAIN_FILE": "<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "ubuntu-local-debug-build",
      "configurePreset": "ubuntu-local-debug",
      "jobs": 22
    },
    {
      "name": "ubuntu-local-debug-build-only-libs",
      "configurePreset": "ubuntu-local-debug",
      "jobs": 22,
      "targets": ["karin-graphics", "karin-system"]
    },
    {
      "name": "ubuntu-local-debug-build-clean",
      "configurePreset": "ubuntu-local-debug",
      "jobs": 22,
      "cleanFirst": true
    }
  ]
}
```

and then
```shell
cmake --preset ubuntu-local-debug
cmake --build --preset ubuntu-local-debug-build
```