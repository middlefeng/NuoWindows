

#include "NuoRenderTargetSwapChain.h"




NuoRenderTargetSwapChain::NuoRenderTargetSwapChain(const PNuoDevice& device,
                                                   const PNuoResourceSwapChain& renderTargets,
                                                   unsigned int sampleCount)
    : _device(device),
      _resources(renderTargets),
      _sampleCount(sampleCount)
{
    _rtvHeap = device->CreateRenderTargetHeap(_resources->Count());

    if (sampleCount > 1)
        CreateSampleResources();

    for (UINT n = 0; n < _resources->Count(); n++)
    {
        // retrieve the n-th handle from _rtvHeap
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DxRenderTargetView(n);

        // create a rtv upon the handle for the n-th buffer in _resources
        PNuoResource resource = (*_resources)[n];
        if (sampleCount > 1)
            resource = (*_sampleResources)[n];

        device->DxDevice()->CreateRenderTargetView(resource->DxResource(), nullptr, rtvHandle);
    }

    _dsvHeap = device->CreateDepthStencilHeap();
    _depthStencil = device->CreateDepthStencil((*_resources)[0]->Width(), (*_resources)[0]->Height(), sampleCount);

    D3D12_DEPTH_STENCIL_VIEW_DESC dsv = {};
    dsv.Format = DXGI_FORMAT_D32_FLOAT;
    dsv.ViewDimension = sampleCount > 1 ? D3D12_DSV_DIMENSION_TEXTURE2DMS : D3D12_DSV_DIMENSION_TEXTURE2D;
    dsv.Texture2D.MipSlice = 0;
    dsv.Flags = D3D12_DSV_FLAG_NONE;

    device->DxDevice()->CreateDepthStencilView(_depthStencil->DxResource(), &dsv, _dsvHeap->DxHeapCPUHandle());

    _renderTargets.resize(renderTargets->Count());
    for (UINT n = 0; n < _resources->Count(); n++)
    {
        PNuoResource resource = (*_resources)[n];
        D3D12_CPU_DESCRIPTOR_HANDLE view = DxRenderTargetView(n);

        PNuoRenderTarget target = std::make_shared<NuoRenderTarget>();
        target->_resource = resource;
        target->_sampleResource = _sampleResources ? (*_sampleResources)[n] : nullptr;
        target->_view = view;

        target->_depthResource = _depthStencil;
        target->_depthView = DxDepthStencilView();

        _renderTargets[n] = target;
    }
}


PNuoRenderTarget NuoRenderTargetSwapChain::RenderTarget(unsigned int inFlight)
{
    return _renderTargets[inFlight];
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoRenderTargetSwapChain::DxRenderTargetView(unsigned int inFlight)
{
   return _rtvHeap->DxRenderTargetView(inFlight);
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoRenderTargetSwapChain::DxDepthStencilView()
{
    return _dsvHeap->DxHeapCPUHandle();
}


void NuoRenderTargetSwapChain::CreateSampleResources()
{
    PNuoDevice device = _device.lock();

    std::vector<PNuoResource> sampleResources(_resources->Count());
    for (UINT i = 0; i < _resources->Count(); ++i)
    {
        PNuoResource resource = (*_resources)[i];
        PNuoResource sampleResource = device->CreateTexture(resource->Format(),
                                                            resource->Width(), resource->Height(),
                                                            _sampleCount);
        sampleResources[i] = sampleResource;
    }

    _sampleResources = std::make_shared<NuoResourceSwapChain>(sampleResources);
}


