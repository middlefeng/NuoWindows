

#include "NuoDirectView.h"
#include "NuoDirect/NuoDevice.h"

#include "NuoDirect/NuoResourceSwapChain.h"
#include "NuoDirect/NuoRenderTargetSwapChain.h"
#include "NuoDirect/NuoFenceSwapChain.h"
#include "NuoRender/NuoRenderPipeline.h"

#include <cassert>



class NuoSwapChain : public std::enable_shared_from_this<NuoSwapChain>
{
    Microsoft::WRL::ComPtr<IDXGISwapChain3> _swapChain;
    Microsoft::WRL::ComPtr<ID3D12Fence> _dxFence;
    WPNuoDevice _device;
    WPNuoDirectView _view;

    PNuoCommandSwapChain _commandSwapChain;
    PNuoResourceSwapChain _buffer;
    PNuoRenderTargetSwapChain _renderTargetSwapChain;
    PNuoFenceSwapChain _fence;
    
    int _currentBackBufferIndex;
    int _presentedBufferIndex;

    DXGI_FORMAT _format;

public:

    NuoSwapChain(const PNuoDirectView& view,
                 DXGI_FORMAT format,
                 unsigned int frameCount,
                 unsigned int sample,
                 unsigned int w, unsigned int h);

    PNuoResourceSwapChain Buffer();
    PNuoRenderTargetSwapChain RenderTargetSwapChain();
    PNuoCommandSwapChain CommandBuffers();

    void ResizeBuffer(unsigned int w, unsigned int h, unsigned int sample);

    void Present();
    void PresentWithoutFence();
    void WaitForGPU();
    void PrepareFrame();

    unsigned int CurrentBackBufferIndex();
    unsigned int BuffersCount();
    unsigned int SampleCount();
    DXGI_FORMAT Format();

private:

    void UpdateBuffer(unsigned int sample);

};


NuoSwapChain::NuoSwapChain(const PNuoDirectView& view,
                           DXGI_FORMAT format,
                           unsigned int frameCount, unsigned int sample,
                           unsigned int w, unsigned int h)
    : _view(view),
      _currentBackBufferIndex(-1),
      _presentedBufferIndex(-1)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = frameCount;
    swapChainDesc.Width = w;
    swapChainDesc.Height = h;
    swapChainDesc.Format = format;
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

    UpdateBuffer(sample);

    _commandSwapChain = std::make_shared<NuoCommandSwapChain>(queue, frameCount);
}


void NuoSwapChain::ResizeBuffer(unsigned int w, unsigned int h, unsigned int sample)
{
    PNuoDirectView view = _view.lock();
    PNuoCommandQueue queue = view->CommandQueue();
    PNuoDevice device = queue->Device();

    unsigned int buffersCount = BuffersCount();

    // release all resources assoicated to the swap chain to make the buffer
    // resize possible.
    //
    _buffer.reset();
    _renderTargetSwapChain.reset();
    _fence = device->CreateFenceSwapChain(buffersCount);
    _currentBackBufferIndex = -1;
    _presentedBufferIndex = -1;
    _commandSwapChain.reset();
    
    HRESULT hr = _swapChain->ResizeBuffers(buffersCount, w, h, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    assert(hr == S_OK);

    UpdateBuffer(sample);
}


void NuoSwapChain::UpdateBuffer(unsigned int sample)
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
    _renderTargetSwapChain.reset(new NuoRenderTargetSwapChain(queue->Device(), (*_buffer)[0]->Format(), _buffer, sample));
    _commandSwapChain = std::make_shared<NuoCommandSwapChain>(queue, buffersCount);
}


PNuoResourceSwapChain NuoSwapChain::Buffer()
{
    return _buffer;
}


PNuoRenderTargetSwapChain NuoSwapChain::RenderTargetSwapChain()
{
    return _renderTargetSwapChain;
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


unsigned int NuoSwapChain::SampleCount()
{
    DXGI_SWAP_CHAIN_DESC1 desc;
    _swapChain->GetDesc1(&desc);

    return desc.SampleDesc.Count;
}


DXGI_FORMAT NuoSwapChain::Format()
{
    DXGI_SWAP_CHAIN_DESC1 desc;
    _swapChain->GetDesc1(&desc);

    return desc.Format;
}


void NuoSwapChain::Present()
{
    PNuoDirectView view = _view.lock();
    _fence->Present(view);
}


void NuoSwapChain::PresentWithoutFence()
{
    _presentedBufferIndex = _currentBackBufferIndex;

    _swapChain->Present(1 /* wait for v-sync */ , 0 /* not allow tearing */ );
    _currentBackBufferIndex = -1;
}


void NuoSwapChain::WaitForGPU()
{
    PNuoDirectView view = _view.lock();
    _fence->WaitForGPU(view);
}


void NuoSwapChain::PrepareFrame()
{
    if (_presentedBufferIndex < 0)
        return;

    PNuoDirectView view = _view.lock();
    _fence->PrepareFrame(view);
}



NuoDirectView::NuoDirectView(const PNuoDevice& device,
                             const PNuoWindow& parent)
	: NuoView(parent),
      _sampleCount(1)
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
    _swapChain = std::make_shared<NuoSwapChain>(view, DXGI_FORMAT_R8G8B8A8_UNORM,
                                                frameCount, _sampleCount, w, h);
}



void NuoDirectView::SetSampleCount(unsigned int sampleCount)
{
    if (_sampleCount == sampleCount)
    {
        return;
    }

    _sampleCount = sampleCount;

    if (_swapChain)
    {
        NuoRect<long> rect = ClientRectDevice();

        WaitForGPU();
        _swapChain->ResizeBuffer(rect.W(), rect.H(), _sampleCount);
    }
}


DXGI_FORMAT NuoDirectView::Format()
{
    return _swapChain->Format();
}


PNuoRenderTarget NuoDirectView::RenderTarget(unsigned int inFlight)
{
    auto swapChain = _swapChain->RenderTargetSwapChain();
    return swapChain->RenderTarget(inFlight);
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


void NuoDirectView::PresentWithoutFence()
{
    _swapChain->PresentWithoutFence();
}


void NuoDirectView::WaitForGPU()
{
    _swapChain->WaitForGPU();
}


unsigned int NuoDirectView::CurrentBackBufferIndex()
{
    return _swapChain->CurrentBackBufferIndex();
}


unsigned int NuoDirectView::BuffersCount()
{
    return _swapChain->BuffersCount();
}


void NuoDirectView::SetRenderPasses(const std::vector<PNuoRenderPass>& passes)
{
    _renderPipeline = std::make_shared<NuoRenderPipeline>();
    _renderPipeline->SetRenderPasses(passes);
}


void NuoDirectView::UpdateRenderPassesDrawable()
{
    const PNuoRenderTarget& renderTarget = RenderTarget(0);
    const auto size = renderTarget->DrawableSize();

    _renderPipeline->SetDrawableSize(size);
}


void NuoDirectView::Render(const PNuoCommandBuffer& commandBuffer)
{
    PNuoRenderTarget target = CurrentRenderTarget();

    _renderPipeline->SetRenderTarget(target);
    _renderPipeline->RenderWithCommandBuffer(commandBuffer);
    _renderPipeline->SetRenderTarget(nullptr);
}

void NuoDirectView::OnPaint()
{
    // check the fence to throttle the encoding according to GPU swap-chain
    //
    _swapChain->PrepareFrame();

    PNuoCommandBuffer commandBuffer = CreateCommandBuffer(true);
    Render(commandBuffer);
    commandBuffer->Commit();

    // preset with setting the fence
    //
    Present();
}


void NuoDirectView::OnSize(unsigned int x, unsigned int y)
{
    NuoRect<long> rect = ClientRectDevice();

    if (_swapChain)
    {
        WaitForGPU();
        _swapChain->ResizeBuffer(rect.W(), rect.H(), _sampleCount);
    }
    else
    {
        CreateSwapChain(3, rect.W(), rect.H());
    }

    NuoView::OnSize(x, y);
}


void NuoDirectView::OnDestroy()
{
    WaitForGPU();

    NuoControl::OnDestroy();
}

