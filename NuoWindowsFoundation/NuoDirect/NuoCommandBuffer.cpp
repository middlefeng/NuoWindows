

#include "NuoCommandBuffer.h"
#include "NuoRenderTarget.h"

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
	buffer->_commandQueue = _commandQueue;
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

	_encoders.push_back(result);

	return result;
}


void NuoCommandEncoder::CopyResource(const PNuoResource& src, const PNuoResource& dst)
{
	auto commandList = *(_commandList.end() - 1);
	commandList->CopyResource(dst->DxResource(), src->DxResource());

	ResourceBarrier(dst, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
}


void NuoCommandBuffer::CopyResource(const PNuoResource& src, const PNuoResource& dst)
{
	PNuoCommandEncoder encoder = CreateRenderPassEncoder();
	encoder->SetPipeline(nullptr);
	encoder->CopyResource(src, dst);
	encoder->EndEncoding();
}


void NuoCommandBuffer::Commit()
{
	for (auto encoder : _encoders)
		encoder->Commit();

	_encoders.clear();
}


void NuoCommandEncoder::SetPipeline(const PNuoPipelineState& pipeline)
{
	if (_commandList.size())
	{
		EndEncoding();
	}

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	PNuoDevice device = _commandQueue->Device();

	auto dxPipeline = pipeline ? pipeline->DxPipeline() : nullptr;
	device->DxDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(),
										  dxPipeline, IID_PPV_ARGS(&commandList));

	_commandList.push_back(commandList);

	if (pipeline)
		commandList->SetGraphicsRootSignature(pipeline->DxRootSignature());

	if (_renderTarget)
	{
		ResourceBarrier(_renderTarget->Resource(),
						D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		commandList->OMSetRenderTargets(1, &_renderTarget->View(), false, nullptr);
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}


void NuoCommandEncoder::UseDefaultViewPort()
{
	auto currentBuffer = *(_commandList.end() - 1);
	PNuoResource resource = _renderTarget->Resource();

	D3D12_VIEWPORT viewPort = { 0, 0, (float)resource->Width(), (float)resource->Height(), 0, 1 };
	D3D12_RECT scissor = { 0, 0, resource->Width(), resource->Height() };
	currentBuffer->RSSetViewports(1, &viewPort);
	currentBuffer->RSSetScissorRects(1, &scissor);
}


void NuoCommandEncoder::EndEncoding()
{
	auto lastBuffer = *(_commandList.end() - 1);

	if (_renderTarget)
	{
		ResourceBarrier(_renderTarget->Resource(),
						D3D12_RESOURCE_STATE_RENDER_TARGET,
						D3D12_RESOURCE_STATE_PRESENT);
	}

	lastBuffer->Close();
}


void NuoCommandEncoder::Commit()
{
	std::vector<ID3D12CommandList*> commandList;
	for (size_t i = 0; i < _commandList.size(); ++i)
		commandList.push_back(_commandList[i].Get());

	_commandQueue->DxQueue()->ExecuteCommandLists(_commandList.size(), &commandList[0]);
}


void NuoCommandEncoder::ResourceBarrier(const PNuoResource& resource,
									    D3D12_RESOURCE_STATES before,
										D3D12_RESOURCE_STATES after)
{
	auto lastBuffer = *(_commandList.end() - 1);

	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource->DxResource();
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	lastBuffer->ResourceBarrier(1, &barrier);
}


