

#include "NuoDevice.h"
#include "NuoStrings.h"
#include "NuoResource.h"

#include <cassert>


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


PNuoResource NuoDevice::CreateBuffer(void* data, size_t size)
{
    return CreateBufferInternal(data, size);
}


PNuoResource NuoDevice::CreateBuffer(size_t size)
{
    return CreateBufferInternal(nullptr, size);
}


PNuoResource NuoDevice::CreateBufferInternal(void* data, size_t size)
{
    D3D12_HEAP_PROPERTIES heapProps;
    heapProps.Type = data ? D3D12_HEAP_TYPE_UPLOAD : D3D12_HEAP_TYPE_DEFAULT;
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProps.CreationNodeMask = 1;
    heapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resourceDesc;
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = size;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    Microsoft::WRL::ComPtr<ID3D12Resource> intermediate;
    HRESULT hr = _dxDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
                                                    &resourceDesc,
                                                    data ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_COPY_DEST,
                                                    nullptr, IID_PPV_ARGS(&intermediate));
    assert(hr == S_OK);

    if (data)
    {
        // Copy the triangle data to the vertex buffer.
        UINT8* pVertexDataBegin;
        D3D12_RANGE readRange = { 0, 0 };        // We do not intend to read from this resource on the CPU.
        hr = intermediate->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
        memcpy(pVertexDataBegin, data, size);
        intermediate->Unmap(0, nullptr);

        assert(hr == S_OK);
    }

    PNuoResource resource = std::make_shared<NuoResource>();
    resource->SetResource(intermediate);

    return resource;
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

