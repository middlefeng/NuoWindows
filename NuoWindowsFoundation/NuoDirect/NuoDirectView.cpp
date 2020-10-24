

#include "NuoDirectView.h"
#include "NuoDirect/NuoDevice.h"

#include "NuoDirect/NuoResourceSwapChain.h"
#include "NuoDirect/NuoRenderTargetSwapChain.h"
#include "NuoDirect/NuoFenceSwapChain.h"



class NuoSwapChain : public std::enable_shared_from_this<NuoSwapChain>
{
    Microsoft::WRL::ComPtr<IDXGISwapChain3> _swapChain;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
    WPNuoDevice _device;
    WPNuoDirectView _view;


    PNuoResourceSwapChain _buffer;
    PNuoRenderTargetSwapChain _rtvSwapChain;
    PNuoFenceSwapChain _fence;
    

    int _currentBackBufferIndex;

public:

    NuoSwapChain(const PNuoDirectView& view,
                 unsigned int frameCount,
                 unsigned int w, unsigned int h);

    PNuoResourceSwapChain Buffer();
    PNuoRenderTargetSwapChain RenderTargetViews();

    void Present();
    void WaitForGPU();
    void MoveToNextFrame();

    // TODO:
    unsigned int CurrentBackBufferIndex();
    IDXGISwapChain3* DxChain() const;
};


NuoSwapChain::NuoSwapChain(const PNuoDirectView& view,
                           unsigned int frameCount,
                           unsigned int w, unsigned int h)
    : _view(view),
      _currentBackBufferIndex(-1)
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

    factory->MakeWindowAssociation(view->Handle(), DXGI_MWA_NO_ALT_ENTER);

    std::vector<PNuoResource> buffers;
    for (unsigned int i = 0; i < frameCount; ++i)
    {
        Microsoft::WRL::ComPtr<ID3D12Resource> dxResource = nullptr;
        _swapChain->GetBuffer(i, IID_PPV_ARGS(&dxResource));

        PNuoResource buffer = std::make_shared<NuoResource>();
        buffer->_dxResources = dxResource;

        buffers.push_back(buffer);
    }

    _buffer = std::make_shared<NuoResourceSwapChain>(buffers);
    _rtvSwapChain.reset(new NuoRenderTargetSwapChain(queue->Device(), _buffer));

    _fence = device->CreateFenceSwapChain(frameCount);
}


PNuoResourceSwapChain NuoSwapChain::Buffer()
{
    return _buffer;
}


PNuoRenderTargetSwapChain NuoSwapChain::RenderTargetViews()
{
    return _rtvSwapChain;
}


unsigned int NuoSwapChain::CurrentBackBufferIndex()
{
    if (_currentBackBufferIndex < 0)
        _currentBackBufferIndex = _swapChain->GetCurrentBackBufferIndex();

    return _currentBackBufferIndex;
}


void NuoSwapChain::Present()
{
    _swapChain->Present(1 /* wait for v-sync */ , 0 /* not allow tearing */ );
    _currentBackBufferIndex = -1;
}


void NuoSwapChain::WaitForGPU()
{
    PNuoDirectView view = _view.lock();
    _fence->WaitForGPU(view);
}


void NuoSwapChain::MoveToNextFrame()
{
    PNuoDirectView view = _view.lock();
    _fence->MoveToNextFrame(view);
}


IDXGISwapChain3* NuoSwapChain::DxChain() const
{
    return _swapChain.Get();
}



NuoDirectView::NuoDirectView(const PNuoWindow& parent)
	: NuoView(parent)
{
}


PNuoCommandQueue NuoDirectView::CommandQueue() const
{
    return _commandQueue;
}


void NuoDirectView::CreateSwapChain(const PNuoDevice& device,
                                    unsigned int frameCount,
                                    unsigned int w, unsigned int h)
{
    PNuoDirectView view = std::dynamic_pointer_cast<NuoDirectView>(shared_from_this());

    _commandQueue = std::make_shared<NuoCommandQueue>(device);
    _swapChain = std::make_shared<NuoSwapChain>(view, frameCount, w, h);
}


PNuoResource NuoDirectView::RenderTarget(unsigned int inFlight)
{
    PNuoResourceSwapChain buffers = _swapChain->Buffer();
    return (*buffers)[inFlight];
}


PNuoResource NuoDirectView::CurrentRenderTarget()
{
    unsigned int current = _swapChain->CurrentBackBufferIndex();
    return RenderTarget(current);
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoDirectView::RenderTargetView(unsigned int inFlight)
{
    auto rtvSwapChain = _swapChain->RenderTargetViews();
    return rtvSwapChain->DxRenderTargetView(inFlight);
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoDirectView::CurrentRenderTargetView()
{
    unsigned int current = _swapChain->CurrentBackBufferIndex();
    return RenderTargetView(current);
}


void NuoDirectView::Present()
{
    _swapChain->Present();
}


void NuoDirectView::WaitForGPU()
{
    _swapChain->WaitForGPU();
}


void NuoDirectView::MoveToNextFrame()
{
    _swapChain->MoveToNextFrame();
}


unsigned int NuoDirectView::CurrentBackBufferIndex()
{
    return _swapChain->CurrentBackBufferIndex();
}



