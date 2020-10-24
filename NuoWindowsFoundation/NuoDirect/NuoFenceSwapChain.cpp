

#include "NuoFenceSwapChain.h"

#include "NuoDirectView.h"
#include "NuoCommandQueue.h"


NuoFenceSwapChain::NuoFenceSwapChain(unsigned int frameCount)
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

    // Schedule a Signal command in the queue.
    queue->DxQueue()->Signal(_fence.Get(), _fenceValues[currentBufferIndex]);

    // Wait until the fence has been processed.
    _fence->SetEventOnCompletion(_fenceValues[currentBufferIndex], _fenceEvent);
    WaitForSingleObjectEx(_fenceEvent, INFINITE, FALSE);

    // Increment the fence value for the current frame.
    _fenceValues[currentBufferIndex]++;
}


void NuoFenceSwapChain::MoveToNextFrame(const PNuoDirectView& view)
{
    PNuoCommandQueue queue = view->CommandQueue();
    unsigned int currentFrameIndex = view->CurrentBackBufferIndex();

    view->Present();

    // Schedule a Signal command in the queue.
    const UINT64 currentFenceValue = _fenceValues[currentFrameIndex];
    queue->DxQueue()->Signal(_fence.Get(), currentFenceValue);

    unsigned int nextFrameIndex = view->CurrentBackBufferIndex();

    // If the next frame is not ready to be rendered yet, wait until it is ready.
    if (_fence->GetCompletedValue() < _fenceValues[nextFrameIndex])
    {
        _fence->SetEventOnCompletion(_fenceValues[nextFrameIndex], _fenceEvent);
        WaitForSingleObjectEx(_fenceEvent, INFINITE, FALSE);
    }

    // Set the fence value for the next frame.
    _fenceValues[nextFrameIndex] = currentFenceValue + 1;
}
