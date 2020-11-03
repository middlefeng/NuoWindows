

#include "NuoCommandBuffer.h"
#include "NuoRenderTarget.h"

#include <windows.h>
#include <cassert>



NuoViewport::NuoViewport()
{
	_viewport = { 0, 0, 0, 0, 0, 1.0f };
}


NuoViewport::NuoViewport(float topLeftX, float topLeftY,
				 		 float width, float height,
						 float minDepth, float maxDepth)
{
	_viewport = { topLeftX, topLeftY, width, height, minDepth, maxDepth };
}



NuoCommandSwapChain::NuoCommandSwapChain(const PNuoCommandQueue& commandQueue, unsigned int frameCount)
	: _commandQueue(commandQueue)
{
	_commandAllocators.resize(frameCount);

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


PNuoCommandBuffer NuoCommandSwapChain::CreateCommandBuffer(unsigned int inFlight, bool resetAllocator)
{
	PNuoCommandBuffer buffer = std::make_shared<NuoCommandBuffer>();
	buffer->_commandQueue = _commandQueue;
	buffer->_commandAllocator = _commandAllocators[inFlight];
	buffer->_inFlight = inFlight;

	if (resetAllocator)
		_commandAllocators[inFlight]->Reset();

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


PNuoCommandQueue NuoCommandBuffer::CommandQueue() const
{
	return _commandQueue;
}



void NuoCommandEncoder::SetClearColor(const NuoVector4& color)
{
	_clearColor = color;
}


void NuoCommandEncoder::SetViewport(const NuoViewport& viewport)
{
	_viewport = viewport;
}


void NuoCommandEncoder::SetConstant(unsigned int index, size_t size, void* constant)
{
	auto commandList = *(_commandList.end() - 1);

	commandList->SetGraphicsRoot32BitConstants(index, size / 4, constant, 0);
}


void NuoCommandEncoder::SetPipeline(const PNuoPipelineState& pipeline)
{
	if (_commandList.size())
		EndEncoding();

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

		commandList->ClearRenderTargetView(_renderTarget->View(), _clearColor._float, 0, nullptr);
		commandList->ClearDepthStencilView(_renderTarget->DepthView(), D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr);

		commandList->OMSetRenderTargets(1, &_renderTarget->View(), false, &_renderTarget->DepthView());
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D12_VIEWPORT viewport = _viewport._viewport;
		if (viewport.Width == 0)
		{
			PNuoResource resource = _renderTarget->Resource();
			viewport.Width = resource->Width();
			viewport.Height = resource->Height();
		}

		D3D12_RECT scissor = { 0, 0, viewport.Width, viewport.Height };
		commandList->RSSetViewports(1, &viewport);
		commandList->RSSetScissorRects(1, &scissor);
	}
}


void NuoCommandEncoder::SetVertexBuffer(const PNuoVertexBuffer& vertexBuffer)
{
	auto commandList = *(_commandList.end() - 1);

	commandList->IASetVertexBuffers(0, 1, vertexBuffer->View());
}


void NuoCommandEncoder::DrawInstanced(unsigned int vertexCount, unsigned int instance)
{
	auto commandList = *(_commandList.end() - 1);

	commandList->DrawInstanced(vertexCount, instance, 0, 0);
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
	_commandList.clear();
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


