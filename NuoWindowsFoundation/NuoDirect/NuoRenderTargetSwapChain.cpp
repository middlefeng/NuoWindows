

#include "NuoRenderTargetSwapChain.h"




NuoRenderTargetSwapChain::NuoRenderTargetSwapChain(const PNuoDevice& device, DXGI_FORMAT format,
                                                   const PNuoResourceSwapChain& backBuffers,
                                                   unsigned int sampleCount)
    : _device(device),
      _backBuffers(backBuffers),
      _sampleCount(sampleCount)
{
    PNuoResource backBuffer = (*_backBuffers)[0];
    auto w = backBuffer->Width();
    auto h = backBuffer->Height();

    _rtvHeap = device->CreateRenderTargetHeap(_backBuffers->Count());
    _renderTarget = std::make_shared<NuoRenderTarget>(device, format, sampleCount, true, false);
    _renderTarget->SetDrawableSize(NuoSize(w, h));

    if (sampleCount == 1)
    {
        for (UINT n = 0; n < _backBuffers->Count(); n++)
        {
            // retrieve the n-th handle from _rtvHeap
            D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DxRenderTargetView(n);

            // create a rtv upon the handle for the n-th buffer in _resources
            PNuoResource resource = (*_backBuffers)[n];

            device->DxDevice()->CreateRenderTargetView(resource->DxResource(), nullptr, rtvHandle);
        }
    }
}


PNuoRenderTarget NuoRenderTargetSwapChain::RenderTarget(unsigned int inFlight)
{
    D3D12_CPU_DESCRIPTOR_HANDLE view;
    if (_sampleCount == 1)
        view = DxRenderTargetView(inFlight);

    _renderTarget->SetBackBuffer((*_backBuffers)[inFlight], view);

    return _renderTarget;
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoRenderTargetSwapChain::DxRenderTargetView(unsigned int inFlight)
{
    assert(_sampleCount == 1);

    return _rtvHeap->DxCPUHandle(inFlight);
}


