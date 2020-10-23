

#include "NuoDescriptorHeap.h"
#include "NuoDevice.h"


D3D12_CPU_DESCRIPTOR_HANDLE NuoDescriptorHeap::DxRenderTargetView(unsigned int inFlight)
{
    PNuoDevice device = _device.lock();

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(_heap->GetCPUDescriptorHandleForHeapStart());
    rtvHandle.ptr = (SIZE_T)((char*)rtvHandle.ptr + inFlight * device->RenderTargetDescriptorHandleIncrementSize());

    return rtvHandle;
}


