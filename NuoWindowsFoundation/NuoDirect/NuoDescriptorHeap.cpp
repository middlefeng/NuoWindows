

#include "NuoDescriptorHeap.h"
#include "NuoDevice.h"



NuoDescriptorHeap::NuoDescriptorHeap(NuoDescriptorType type)
    : _type(type), _size(0)
{
}



unsigned int NuoDescriptorHeap::Incremental()
{
    switch (_type)
    {
    case kNuoDescriptor_RenderTarget:
        return _device->RenderTargetDescriptorHandleIncrementSize();
    case kNuoDescriptor_DepthStencil:
        return _device->DxDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    case kNuoDescriptor_ShaderResource:
        return _device->ShaderResourceDescriptorHandleIncrementSize();
    }
}



D3D12_CPU_DESCRIPTOR_HANDLE NuoDescriptorHeap::DxCPUHandle(unsigned int index)
{
    D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle(DxHeapCPUHandle());
    SIZE_T offset = index * Incremental();
    cbvHandle.ptr = (SIZE_T)((char*)cbvHandle.ptr + offset);

    return cbvHandle;
}


D3D12_GPU_DESCRIPTOR_HANDLE NuoDescriptorHeap::DxGPUHandle(unsigned int index)
{
    D3D12_GPU_DESCRIPTOR_HANDLE cbvHandle(DxHeapGPUHandle());
    SIZE_T offset = index * Incremental();
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


