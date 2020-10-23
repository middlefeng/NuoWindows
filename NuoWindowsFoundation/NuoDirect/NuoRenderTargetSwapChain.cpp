

#include "NuoRenderTargetSwapChain.h"




NuoRenderTargetSwapChain::NuoRenderTargetSwapChain(const PNuoDevice& device,
                                                   const PNuoResourceSwapChain& renderTargets,
                                                   unsigned int frameCount)
    : _device(device),
      _resources(renderTargets)
{
    _rtvHeap = device->CreateRenderTargetHeap(frameCount);

    /*D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = frameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    device->DxDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvHeap));

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());*/

    for (UINT n = 0; n < frameCount; n++)
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DxRenderTargetView(n);
        device->DxDevice()->CreateRenderTargetView((*_resources)[n]->DxResource(), nullptr, rtvHandle);
    }
}


/*size_t NuoRenderTargetSwapChain::DescriptorSize()
{
    if (!_descriptorSize)
    {
        PNuoDevice device = _device.lock();
        _descriptorSize = device->
    }

    return _descriptorSize;
}*/


D3D12_CPU_DESCRIPTOR_HANDLE NuoRenderTargetSwapChain::DxRenderTargetView(unsigned int inFlight)
{
   return _rtvHeap->DxRenderTargetView(inFlight);
}


