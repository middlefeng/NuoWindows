

#include "NuoRenderTargetSwapChain.h"




NuoRenderTargetSwapChain::NuoRenderTargetSwapChain(const PNuoDevice& device,
                                                   const PNuoResourceSwapChain& renderTargets)
    : _device(device),
      _resources(renderTargets)
{
    _rtvHeap = device->CreateRenderTargetHeap(_resources->Count());

    for (UINT n = 0; n < _resources->Count(); n++)
    {
        // retrieve the n-th handle from _rtvHeap
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DxRenderTargetView(n);

        // create a rtv upon the handle for the n-th buffer in _resources
        device->DxDevice()->CreateRenderTargetView((*_resources)[n]->DxResource(), nullptr, rtvHandle);
    }
}


PNuoRenderTarget NuoRenderTargetSwapChain::RenderTarget(unsigned int inFlight)
{
    PNuoResource resource = (*_resources)[inFlight];
    D3D12_CPU_DESCRIPTOR_HANDLE view = DxRenderTargetView(inFlight);

    PNuoRenderTarget target = std::make_shared<NuoRenderTarget>();
    target->_resource = resource;
    target->_view = view;

    return target;
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoRenderTargetSwapChain::DxRenderTargetView(unsigned int inFlight)
{
   return _rtvHeap->DxRenderTargetView(inFlight);
}


