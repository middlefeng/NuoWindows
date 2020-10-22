

#include "NuoDirectView.h"
#include "NuoDirect/NuoDevice.h"




NuoSwapChain::NuoSwapChain(const PNuoDirectView& view,
                           unsigned int frameCount,
                           unsigned int w, unsigned int h)
    : _view(view)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = frameCount;
    swapChainDesc.Width = w;
    swapChainDesc.Height = h;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    PNuoCommandQueue queue = view->CommandQueue();
    PNuoDevice device = queue->Device();

    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
    auto factory = device->DxFactory();
    factory->CreateSwapChainForHwnd(queue->DxQueue(), view->Handle(),
                                    &swapChainDesc, nullptr, nullptr, &swapChain);
    
    if (swapChain)
        swapChain.As(&_swapChain);
}



NuoDirectView::NuoDirectView(const PNuoWindow& parent)
	: NuoView(parent)
{   
}


void NuoDirectView::SetCommandQueue(const PNuoCommandQueue& queue)
{
    _commandQueue = queue;
}


PNuoCommandQueue NuoDirectView::CommandQueue() const
{
    return _commandQueue;
}


void NuoDirectView::CreateSwapChain(unsigned int frameCount,
                                    unsigned int w, unsigned int h)
{
    PNuoDirectView view = std::dynamic_pointer_cast<NuoDirectView>(shared_from_this());

    _swapChain = std::make_shared<NuoSwapChain>(view, frameCount, w, h);
}


