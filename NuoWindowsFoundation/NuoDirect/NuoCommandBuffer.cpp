

#include "NuoCommandBuffer.h"

#include "NuoRenderTarget.h"
#include "NuoResourceSwapChain.h"

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
	_inFlightBuffers.resize(frameCount);
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


unsigned int NuoRenderInFlight::FrameCount()
{
	return _frameCount;
}


void NuoRenderInFlight::SetInFlight(unsigned int inFlight, unsigned int frameCount)
{
	_inFlight = inFlight;
	_frameCount = frameCount;
}


PNuoCommandBuffer NuoCommandSwapChain::CreateCommandBuffer(unsigned int inFlight, bool resetAllocator)
{
	_inFlightBuffers[inFlight].clear();

	PNuoCommandBuffer buffer = std::make_shared<NuoCommandBuffer>();
	buffer->_commandQueue = _commandQueue;
	buffer->_commandAllocator = _commandAllocators[inFlight];
	buffer->SetInFlight(inFlight, (unsigned int)_inFlightBuffers.size());

	_inFlightBuffers[inFlight].push_back(buffer);

	if (resetAllocator)
		_commandAllocators[inFlight]->Reset();

	return buffer;
}


PNuoCommandEncoder NuoCommandBuffer::CreateRenderPassEncoder()
{
	PNuoDevice device = _commandQueue->Device();

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	device->DxDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(),
										  nullptr, IID_PPV_ARGS(&commandList));

	PNuoCommandEncoder result = std::make_shared<NuoCommandEncoder>();
	result->_commandList = commandList;
	result->SetInFlight(_inFlight, _frameCount);

	_encoders.push_back(result);

	return result;
}


void NuoCommandEncoder::CopyResource(const PNuoResource& src, const PNuoResource& dst)
{
	_commandList->CopyResource(dst->DxResource(), src->DxResource());

	ResourceBarrier(dst, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
}


void NuoCommandBuffer::CopyResource(const PNuoResource& src, const PNuoResource& dst)
{
	PNuoCommandEncoder encoder = CreateRenderPassEncoder();
	encoder->CopyResource(src, dst);
	encoder->EndEncoding();
}


void NuoCommandBuffer::Commit()
{
	std::vector<ID3D12CommandList*> commands;

	for (auto encoder : _encoders)
	{
		ID3D12GraphicsCommandList* commandList = encoder->DxEncoder();
		commands.push_back(commandList);
	}

	_commandQueue->DxQueue()->ExecuteCommandLists((UINT)commands.size(), commands.data());
}


PNuoCommandQueue NuoCommandBuffer::CommandQueue() const
{
	return _commandQueue;
}



void NuoCommandEncoder::SetClearColor(const NuoVectorFloat4& color)
{
	float acolor[] = { color.x(), color.y(), color.z(), color.w() };

	_commandList->ClearRenderTargetView(_renderTarget->View(), acolor, 0, nullptr);
	_commandList->ClearDepthStencilView(_renderTarget->DepthView(), D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr);
}


void NuoCommandEncoder::SetViewport(const NuoViewport& viewport)
{
	PNuoResource renderBuffer = _renderTarget->RenderBuffer();
	FLOAT w = (FLOAT)renderBuffer->Width();
	FLOAT h = (FLOAT)renderBuffer->Height();

	D3D12_VIEWPORT viewport_ = viewport._viewport;
	if (viewport_.Width == 0)
	{
		viewport_.Width = w;
		viewport_.Height = h;
	}

	D3D12_RECT scissor = { 0, 0, (LONG)w, (LONG)h };
	_commandList->RSSetViewports(1, &viewport_);
	_commandList->RSSetScissorRects(1, &scissor);
}


void NuoCommandEncoder::SetRootConstant(unsigned int index, size_t size, void* constant)
{
	_commandList->SetGraphicsRoot32BitConstants(index, (UINT)size / 4, constant, 0);
}


void NuoCommandEncoder::SetRootConstantBuffer(unsigned int index, const PNuoResourceSwapChain& cb)
{
	const D3D12_GPU_VIRTUAL_ADDRESS addr = cb->GPUAddress(InFlight());
	_commandList->SetGraphicsRootConstantBufferView(index, addr);
}


void NuoCommandEncoder::SetRenderTarget(const PNuoRenderTarget& renderTarget)
{
	_renderTarget = renderTarget;

	_commandList->OMSetRenderTargets(1, &_renderTarget->View(), false, &_renderTarget->DepthView());
	_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


void NuoCommandEncoder::SetPipeline(const PNuoPipelineState& pipeline)
{
	ID3D12PipelineState* dxPipeline = pipeline->DxPipeline();
	
	_commandList->SetPipelineState(dxPipeline);
	if (pipeline->DxRootSignature())
	_commandList->SetGraphicsRootSignature(pipeline->DxRootSignature());
}


void NuoCommandEncoder::SetVertexBuffer(const PNuoVertexBuffer& vertexBuffer)
{
	_commandList->IASetVertexBuffers(0, 1, vertexBuffer->View());
	_commandList->IASetIndexBuffer(vertexBuffer->IndiciesView());
}


void NuoCommandEncoder::DrawIndexed(unsigned int indiciesCount)
{
	_commandList->DrawIndexedInstanced(indiciesCount, 1, 0, 0, 0);
}



void NuoCommandEncoder::EndEncoding()
{
	_commandList->Close();
}


ID3D12GraphicsCommandList* NuoCommandEncoder::DxEncoder()
{
	return _commandList.Get();
}


void NuoCommandEncoder::ResourceBarrier(const PNuoResource& resource,
									    D3D12_RESOURCE_STATES before,
										D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource->DxResource();
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	_commandList->ResourceBarrier(1, &barrier);
}


