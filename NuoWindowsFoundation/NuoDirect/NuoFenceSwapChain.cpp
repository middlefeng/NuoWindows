

#include "NuoFenceSwapChain.h"

#include "NuoDirectView.h"
#include "NuoCommandQueue.h"

#include <cassert>
#include <limits>


static const UINT64 kCounterLimit = (UINT64)(UINT64_MAX * 0.9);



NuoFenceSwapChain::NuoFenceSwapChain(unsigned int frameCount)
    : _currentFenceValue(0)
{
	_fenceValues.resize(frameCount, 0);
	_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}


NuoFenceSwapChain::~NuoFenceSwapChain()
{
	CloseHandle(_fenceEvent);
}


void NuoFenceSwapChain::WaitForGPU(const PNuoDirectView& view)
{
    PNuoCommandQueue queue = view->CommandQueue();
    unsigned int currentBufferIndex = view->CurrentBackBufferIndex();

    WaitForGPU(queue, currentBufferIndex);
}


void NuoFenceSwapChain::WaitForGPU(const PNuoCommandQueue& commandQueue)
{
    assert(_fenceValues.size() == 1);

    WaitForGPU(commandQueue, 0);
}


void NuoFenceSwapChain::WaitForGPU(const PNuoCommandQueue& commandQueue, unsigned int inFlight)
{
    // use a value that is greater than that of any in-flight frames
    _currentFenceValue += 1;

    // Schedule a Signal command in the queue.
    commandQueue->DxQueue()->Signal(_fence.Get(), _currentFenceValue + 1);

    // Wait until the fence has been processed.
    _fence->SetEventOnCompletion(_currentFenceValue + 1, _fenceEvent);
    WaitForSingleObjectEx(_fenceEvent, INFINITE, FALSE);

    commandQueue->ReleasePendingCommandBuffers();

    // Reset fence value for the current frame.
    for (auto& value : _fenceValues)
        value = 0;

    _currentFenceValue = 0;
}


void NuoFenceSwapChain::Present(const PNuoDirectView& view)
{
    PNuoCommandQueue queue = view->CommandQueue();
    unsigned int currentFrameIndex = view->CurrentBackBufferIndex();

    view->PresentWithoutFence();

    // Schedule a Signal command in the queue.
    _currentFenceValue = _fenceValues[currentFrameIndex];
    queue->DxQueue()->Signal(_fence.Get(), _currentFenceValue);
}


void NuoFenceSwapChain::PrepareFrame(const PNuoDirectView& view)
{
    if (_currentFenceValue > kCounterLimit)
    {
        WaitForGPU(view);
        return;
    }

    PNuoCommandQueue queue = view->CommandQueue();
    unsigned int nextFrameIndex = view->CurrentBackBufferIndex();

    // If the next frame is not ready to be rendered yet, wait until it is ready.
    if (_fence->GetCompletedValue() < _fenceValues[nextFrameIndex])
    {
        _fence->SetEventOnCompletion(_fenceValues[nextFrameIndex], _fenceEvent);
        WaitForSingleObjectEx(_fenceEvent, INFINITE, FALSE);
    }

    // Set the fence value for the next frame.
    _fenceValues[nextFrameIndex] = _currentFenceValue + 1;
}
