#ifndef KARIN_WINDOWS_DIRECT2D_DXGIDEVICE_H
#define KARIN_WINDOWS_DIRECT2D_DXGIDEVICE_H
#include <d3d11.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include <array>
#include <iostream>

namespace karin
{

class dxgiDevice
{
public:
    static Microsoft::WRL::ComPtr<IDXGIDevice4>& Get()
    {
        static Microsoft::WRL::ComPtr<IDXGIDevice4> device = nullptr;
        if (device == nullptr)
        {
            CreateDevice(&device);
        }

        return device;
    }

private:
    static void CreateDevice(Microsoft::WRL::ComPtr<IDXGIDevice4>* dxgi_device)
    {
        constexpr UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
        constexpr int num_feature_levels = 7;

        std::array<D3D_FEATURE_LEVEL, num_feature_levels> feature_levels
            = {D3D_FEATURE_LEVEL_11_1,
               D3D_FEATURE_LEVEL_11_0,
               D3D_FEATURE_LEVEL_10_1,
               D3D_FEATURE_LEVEL_10_0,
               D3D_FEATURE_LEVEL_9_3,
               D3D_FEATURE_LEVEL_9_2,
               D3D_FEATURE_LEVEL_9_1};

        Microsoft::WRL::ComPtr<ID3D11Device> d3d_device = nullptr;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d_context = nullptr;

        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creation_flags,
            feature_levels.data(),
            feature_levels.size(),
            D3D11_SDK_VERSION,
            &d3d_device,
            nullptr,
            &d3d_context
        );
        if (FAILED(hr))
        {
            std::cout << "Failed to create Direct3D device" << std::endl;
            exit(1);
        }

        hr = d3d_device.As(dxgi_device);
        if (FAILED(hr))
        {
            std::cout << "Failed to get DXGI device" << std::endl;
            exit(1);
        }
    }
};

}  // namespace karin

#endif  // KARIN_WINDOWS_DIRECT2D_DXGIDEVICE_H
