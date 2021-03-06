﻿

#include "NuoFenceSwapChain.h"

#include "NuoDirectView.h"
#include "NuoCommandQueue.h"

#include <cassert>


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

    WaitForGPU(queue, currentBufferIndex);
}


void NuoFenceSwapChain::WaitForGPU(const PNuoCommandQueue& commandQueue)
{
    assert(_fenceValues.size() == 1);

    WaitForGPU(commandQueue, 0);
}


void NuoFenceSwapChain::WaitForGPU(const PNuoCommandQueue& commandQueue, unsigned int inFlight)
{
    // Schedule a Signal command in the queue.
    commandQueue->DxQueue()->Signal(_fence.Get(), _fenceValues[inFlight]);

    // Wait until the fence has been processed.
    _fence->SetEventOnCompletion(_fenceValues[inFlight], _fenceEvent);
    WaitForSingleObjectEx(_fenceEvent, INFINITE, FALSE);

    commandQueue->ReleasePendingCommandBuffers();

    // Increment the fence value for the current frame.
    _fenceValues[inFlight] += 1;
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
