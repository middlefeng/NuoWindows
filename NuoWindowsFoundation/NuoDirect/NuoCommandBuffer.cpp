

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


unsigned int NuoRenderInFlight::InFlight()
{
	return _inFlight;
}


PNuoCommandBuffer NuoCommandSwapChain::CreateCommandBuffer(unsigned int inFlight)
{
	PNuoCommandBuffer buffer = std::make_shared<NuoCommandBuffer>();
	buffer->_commandAllocator = _commandAllocators[inFlight];
	buffer->_inFlight = inFlight;

	return buffer;
}


PNuoCommandEncoder NuoCommandBuffer::CreateRenderPassEncoder()
{
	PNuoCommandEncoder result = std::make_shared<NuoCommandEncoder>();
	result->_commandAllocator = _commandAllocator;
	result->_commandQueue = _commandQueue;
	result->_inFlight = _inFlight;

	return result;
}


void NuoCommandEncoder::SetPipeline(const PNuoPipelineState& pipeline)
{
	if (_commandList.size())
		EndEncoding();

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	PNuoDevice device = _commandQueue->Device();
	device->DxDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(),
										  pipeline->DxPipeline(), IID_PPV_ARGS(&commandList));

	_commandList.push_back(commandList);
}


void NuoCommandEncoder::EndEncoding()
{
	auto lastBuffer = *(_commandList.end() - 1);
	lastBuffer->Close();
}

