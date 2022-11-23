

#include "NuoDescriptorHeap.h"


#include "NuoTexture.h"
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
    return 0;
}



D3D12_CPU_DESCRIPTOR_HANDLE NuoDescriptorHeap::DxCPUHandle(unsigned int index)
{
    D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle(DxHeapCPUHandle());
    SIZE_T offset = (UINT32)index * Incremental();
    cbvHandle.ptr = (SIZE_T)((char*)cbvHandle.ptr + offset);

    return cbvHandle;
}


D3D12_GPU_DESCRIPTOR_HANDLE NuoDescriptorHeap::DxGPUHandle(unsigned int index)
{
    D3D12_GPU_DESCRIPTOR_HANDLE cbvHandle(DxHeapGPUHandle());
    SIZE_T offset = (UINT32)index * Incremental();
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


void NuoDescriptorHeap::SetTexture(unsigned int index,
                                   const PNuoTexture& texture)
{
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = texture->Format();
    srvDesc.ViewDimension = texture->SampleCount() > 1 ? D3D12_SRV_DIMENSION_TEXTURE2DMS :
                                                         D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    _device->DxDevice()->CreateShaderResourceView(texture->DxResource(), &srvDesc, DxCPUHandle(index));
}


void NuoDescriptorHeap::SetTextureMipSlice(unsigned int index,
                                           const PNuoTexture& texture,
                                           unsigned int mipSlice)
{
    D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = texture->Format();
    uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
    uavDesc.Texture2D.MipSlice = mipSlice;

    _device->DxDevice()->CreateUnorderedAccessView(texture->DxResource(), nullptr,
                                                   &uavDesc, DxCPUHandle(index));
}


void NuoDescriptorHeap::SetConstantBuffer(unsigned int index,
                                          const PNuoResource& buffer)
{
    D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
    desc.BufferLocation = buffer->DxResource()->GetGPUVirtualAddress();
    desc.SizeInBytes = buffer->Size();

    _device->DxDevice()->CreateConstantBufferView(&desc, DxCPUHandle(index));
}


ID3D12DescriptorHeap* NuoDescriptorHeap::DxHeap() const
{
    return _heap.Get();
}

