

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

    _dsvHeap = device->CreateDepthStencilHeap();
    _depthStencil = device->CreateDepthStencil((*_resources)[0]->Width(), (*_resources)[0]->Height());

    D3D12_DEPTH_STENCIL_VIEW_DESC dsv = {};
    dsv.Format = DXGI_FORMAT_D32_FLOAT;
    dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsv.Texture2D.MipSlice = 0;
    dsv.Flags = D3D12_DSV_FLAG_NONE;

    device->DxDevice()->CreateDepthStencilView(_depthStencil->DxResource(), &dsv, _dsvHeap->DxHeapCPUHandle());
}


PNuoRenderTarget NuoRenderTargetSwapChain::RenderTarget(unsigned int inFlight)
{
    PNuoResource resource = (*_resources)[inFlight];
    D3D12_CPU_DESCRIPTOR_HANDLE view = DxRenderTargetView(inFlight);

    PNuoRenderTarget target = std::make_shared<NuoRenderTarget>();
    target->_resource = resource;
    target->_view = view;

    target->_depthResource = _depthStencil;
    target->_depthView = DxDepthStencilView();

    return target;
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoRenderTargetSwapChain::DxRenderTargetView(unsigned int inFlight)
{
   return _rtvHeap->DxRenderTargetView(inFlight);
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoRenderTargetSwapChain::DxDepthStencilView()
{
    return _dsvHeap->DxHeapCPUHandle();
}


