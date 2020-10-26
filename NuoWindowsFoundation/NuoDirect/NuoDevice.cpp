

#include "NuoDevice.h"
#include "NuoStrings.h"


std::set<PNuoDevice> NuoDevice::_devices;


std::set<PNuoDevice> NuoDevice::Devices()
{
    if (_devices.size())
        return _devices;

	UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    {
        ID3D12Debug* debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();

            // Enable additional debug layers.
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#endif

    do
    {
        Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
        CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
        if (!factory)
            break;

        Microsoft::WRL::ComPtr<IDXGIFactory6> factory6;
        if (!SUCCEEDED(factory->QueryInterface(IID_PPV_ARGS(&factory6))))
            break;

        bool requestHighPerformanceAdapter = true;
        DXGI_GPU_PREFERENCE preference = requestHighPerformanceAdapter ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE :
                                                                         DXGI_GPU_PREFERENCE_UNSPECIFIED;

        Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
        if (requestHighPerformanceAdapter)
        for (UINT adapterIndex = 0; ; ++adapterIndex)
        {
             HRESULT found = factory6->EnumAdapterByGpuPreference(adapterIndex, preference, IID_PPV_ARGS(&adapter));

             if (found == DXGI_ERROR_NOT_FOUND)
                 break;

             DXGI_ADAPTER_DESC1 desc1;
             adapter->GetDesc1(&desc1);

             Microsoft::WRL::ComPtr<ID3D12Device> dxDevice;
             if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), &dxDevice)))
             {
                 PNuoDevice device = std::make_shared<NuoDevice>();
                 device->_dxDevice = dxDevice;
                 device->_dxDesc = desc1;
                 device->_dxFactor = factory6;

                 _devices.insert(device);
             }
        }
    }
    while (false);

	return _devices;
}


void NuoDevice::EnableDebugInfoQueue()
{
#if defined(_DEBUG)
    Microsoft::WRL::ComPtr<ID3D12InfoQueue> dxInfoQueue;
    if (SUCCEEDED(_dxDevice.As(&dxInfoQueue)))
    {
        dxInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        dxInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
        dxInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
    }
#endif
}


std::string NuoDevice::Name() const
{
    return StringToUTF8(_dxDesc.Description);
}


unsigned int NuoDevice::RenderTargetDescriptorHandleIncrementSize() const
{
    return _dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}



PNuoDescriptorHeap NuoDevice::CreateRenderTargetHeap(unsigned int frameCount)
{
    PNuoDescriptorHeap heap(new NuoDescriptorHeap());

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = frameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    heap->_size = frameCount;
    heap->_device = shared_from_this();
    _dxDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&heap->_heap));

    return heap;
}


PNuoFenceSwapChain NuoDevice::CreateFenceSwapChain(unsigned int frameCount)
{
    PNuoFenceSwapChain result = std::make_shared<NuoFenceSwapChain>(frameCount);

    auto value = result->_fenceValues[0];
    _dxDevice->CreateFence(value, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&result->_fence));
    result->_fenceValues[0] += 1;

    return result;
}


ID3D12Device* NuoDevice::DxDevice() const
{
    return _dxDevice.Get();
}


IDXGIFactory6* NuoDevice::DxFactory() const
{
    return _dxFactor.Get();
}

