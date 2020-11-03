

#include "NuoDescriptorHeap.h"
#include "NuoDevice.h"




D3D12_CPU_DESCRIPTOR_HANDLE NuoDescriptorHeap::DxRenderTargetView(unsigned int inFlight)
{
    PNuoDevice device = _device.lock();

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(_heap->GetCPUDescriptorHandleForHeapStart());
    SIZE_T offset = inFlight * device->RenderTargetDescriptorHandleIncrementSize();
    rtvHandle.ptr = (SIZE_T)((char*)rtvHandle.ptr + offset);

    return rtvHandle;
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoDescriptorHeap::DxHeapCPUHandle()
{
    return _heap->GetCPUDescriptorHandleForHeapStart();
}


