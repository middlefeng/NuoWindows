

#include "NuoDescriptorHeap.h"
#include "NuoDevice.h"




D3D12_CPU_DESCRIPTOR_HANDLE NuoDescriptorHeap::DxRenderTargetView(unsigned int inFlight)
{
    PNuoDevice device = _device.lock();

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(DxHeapCPUHandle());
    SIZE_T offset = inFlight * device->RenderTargetDescriptorHandleIncrementSize();
    rtvHandle.ptr = (SIZE_T)((char*)rtvHandle.ptr + offset);

    return rtvHandle;
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoDescriptorHeap::DxConstantBufferHandle(unsigned int inFlight)
{
    PNuoDevice device = _device.lock();

    D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle(DxHeapCPUHandle());
    SIZE_T offset = inFlight * device->ConstantBufferDescriptorHandleIncrementSize();
    cbvHandle.ptr = (SIZE_T)((char*)cbvHandle.ptr + offset);

    return cbvHandle;
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoDescriptorHeap::DxHeapCPUHandle()
{
    return _heap->GetCPUDescriptorHandleForHeapStart();
}


D3D12_GPU_DESCRIPTOR_HANDLE NuoDescriptorHeap::DxHeapGPUHandle()
{
    return _heap->GetGPUDescriptorHandleForHeapStart();
}


