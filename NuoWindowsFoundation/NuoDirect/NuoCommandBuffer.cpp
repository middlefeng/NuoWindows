

#include "NuoCommandBuffer.h"

#include <windows.h>
#include <cassert>



NuoCommandSwapChain::NuoCommandSwapChain(const PNuoCommandQueue& commandQueue, unsigned int frameCount)
	: _commandQueue(commandQueue)
{
	_commandAllocators.resize(frameCount);
	_commandBuffers.resize(frameCount);

	for (UINT n = 0; n < frameCount; n++)
	{
		HRESULT hr = commandQueue->Device()->DxDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&_commandAllocators[n]));
		assert(hr == S_OK);
	}
}

PNuoCommandQueue NuoCommandSwapChain::CommandQueue() const
{
	return _commandQueue;
}


PNuoCommandBuffer NuoCommandSwapChain::CreateCommandBuffer(unsigned int inFlight)
{
	PNuoCommandBuffer buffer = std::make_shared<NuoCommandBuffer>();
	buffer->_commandAllocator = _commandAllocators[inFlight];
	buffer->_inFlight = inFlight;

	return buffer;
}

