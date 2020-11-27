

#include "NuoDirectView.h"
#include "NuoDirect/NuoDevice.h"

#include "NuoDirect/NuoResourceSwapChain.h"
#include "NuoDirect/NuoRenderTargetSwapChain.h"
#include "NuoDirect/NuoFenceSwapChain.h"

#include <cassert>



class NuoSwapChain : public std::enable_shared_from_this<NuoSwapChain>
{
    Microsoft::WRL::ComPtr<IDXGISwapChain3> _swapChain;
    Microsoft::WRL::ComPtr<ID3D12Fence> _dxFence;
    WPNuoDevice _device;
    WPNuoDirectView _view;

    PNuoCommandSwapChain _commandSwapChain;
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
    PNuoCommandSwapChain CommandBuffers();

    void ResizeBuffer(unsigned int w, unsigned int h);

    void Present();
    void WaitForGPU();
    void MoveToNextFrame();

    unsigned int CurrentBackBufferIndex();
    unsigned int BuffersCount();

private:

    void UpdateBuffer();

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
    HRESULT hr = factory->CreateSwapChainForHwnd(queue->DxQueue(), view->Handle(),
                                                 &swapChainDesc, nullptr, nullptr, &swapChain);
    assert(hr == S_OK);
    
    if (swapChain)
        swapChain.As(&_swapChain);

    factory->MakeWindowAssociation(view->Handle(), DXGI_MWA_NO_ALT_ENTER);
    _fence = device->CreateFenceSwapChain(frameCount);

    UpdateBuffer();

    _commandSwapChain = std::make_shared<NuoCommandSwapChain>(queue, frameCount);
}


void NuoSwapChain::ResizeBuffer(unsigned int w, unsigned int h)
{
    PNuoDirectView view = _view.lock();
    PNuoCommandQueue queue = view->CommandQueue();
    PNuoDevice device = queue->Device();

    unsigned int buffersCount = BuffersCount();

    // release all resources assoicated to the swap chain to make the buffer
    // resize possible.
    //
    _buffer.reset();
    _rtvSwapChain.reset();
    _fence = device->CreateFenceSwapChain(buffersCount);
    _currentBackBufferIndex = -1;
    _commandSwapChain.reset();
    
    HRESULT hr = _swapChain->ResizeBuffers(buffersCount, w, h, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    assert(hr == S_OK);

    UpdateBuffer();
}


void NuoSwapChain::UpdateBuffer()
{
    unsigned int buffersCount = BuffersCount();

    std::vector<PNuoResource> buffers;
    for (unsigned int i = 0; i < buffersCount; ++i)
    {
        Microsoft::WRL::ComPtr<ID3D12Resource> dxResource = nullptr;
        _swapChain->GetBuffer(i, IID_PPV_ARGS(&dxResource));

        PNuoResource buffer = std::make_shared<NuoResource>();
        buffer->SetResource(dxResource, D3D12_RESOURCE_STATE_PRESENT);

        buffers.push_back(buffer);
    }

    PNuoDirectView view = _view.lock();
    PNuoCommandQueue queue = view->CommandQueue();

    _buffer = std::make_shared<NuoResourceSwapChain>(buffers);
    _rtvSwapChain.reset(new NuoRenderTargetSwapChain(queue->Device(), (*_buffer)[0]->Format(), _buffer, 8));
    _commandSwapChain = std::make_shared<NuoCommandSwapChain>(queue, buffersCount);
}


PNuoResourceSwapChain NuoSwapChain::Buffer()
{
    return _buffer;
}


PNuoRenderTargetSwapChain NuoSwapChain::RenderTargetViews()
{
    return _rtvSwapChain;
}


PNuoCommandSwapChain NuoSwapChain::CommandBuffers()
{
    return _commandSwapChain;
}


unsigned int NuoSwapChain::CurrentBackBufferIndex()
{
    if (_currentBackBufferIndex < 0)
        _currentBackBufferIndex = _swapChain->GetCurrentBackBufferIndex();

    return _currentBackBufferIndex;
}


unsigned int NuoSwapChain::BuffersCount()
{
    DXGI_SWAP_CHAIN_DESC desc;
    _swapChain->GetDesc(&desc);

    return desc.BufferCount;
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



NuoDirectView::NuoDirectView(const PNuoDevice& device,
                             const PNuoWindow& parent)
	: NuoView(parent)
{
    _commandQueue = std::make_shared<NuoCommandQueue>(device);
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



PNuoRenderTarget NuoDirectView::RenderTarget(unsigned int inFlight)
{
    auto rtvSwapChain = _swapChain->RenderTargetViews();
    return rtvSwapChain->RenderTarget(inFlight);
}


PNuoRenderTarget NuoDirectView::CurrentRenderTarget()
{
    unsigned int current = _swapChain->CurrentBackBufferIndex();
    return RenderTarget(current);
}


PNuoCommandBuffer NuoDirectView::CreateCommandBuffer(bool resetAllocator)
{
    auto commandBuffers = _swapChain->CommandBuffers();
    unsigned int current = _swapChain->CurrentBackBufferIndex();
    return commandBuffers->CreateCommandBuffer(current, resetAllocator);
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


unsigned int NuoDirectView::BuffersCount()
{
    return _swapChain->BuffersCount();
}


void NuoDirectView::Render(const PNuoCommandBuffer& commandBuffer)
{

}

void NuoDirectView::OnPaint()
{
    PNuoCommandBuffer commandBuffer = CreateCommandBuffer(true);

    Render(commandBuffer);
    
    commandBuffer->Commit();
    MoveToNextFrame();
}


void NuoDirectView::OnSize(unsigned int x, unsigned int y)
{
    NuoRect<long> rect = ClientRectDevice();

    if (_swapChain)
    {
        WaitForGPU();
        _swapChain->ResizeBuffer(rect.W(), rect.H());
    }
    else
    {
        CreateSwapChain(3, rect.W(), rect.H());
    }
}


void NuoDirectView::OnDestroy()
{
    WaitForGPU();

    NuoControl::OnDestroy();
}

