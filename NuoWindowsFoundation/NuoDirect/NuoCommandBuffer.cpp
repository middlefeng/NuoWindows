

#include "NuoCommandBuffer.h"

#include "NuoTexture.h"
#include "NuoRenderTarget.h"
#include "NuoResourceSwapChain.h"

#include <windows.h>
#include <cassert>
#include "d3dx12.h"



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


unsigned int NuoRenderInFlight::InFlight() const
{
	return _inFlight;
}


unsigned int NuoRenderInFlight::FrameCount() const
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
	result->_commandQueue = _commandQueue;
	result->SetInFlight(_inFlight, _frameCount);

	_encoders.push_back(result);

	return result;
}


void NuoCommandEncoder::CopyResource(const PNuoResource& src, const PNuoResource& dst)
{
	ResourceBarrier(dst, D3D12_RESOURCE_STATE_COPY_DEST);

	_commandList->CopyResource(dst->DxResource(), src->DxResource());

	ResourceBarrier(dst, D3D12_RESOURCE_STATE_GENERIC_READ);
}


void NuoCommandEncoder::CopyTexture(const std::vector<UINT8>& src, std::vector<PNuoResource>& intermediate, const PNuoTexture& texture)
{
	PNuoDevice device = _commandQueue->Device();
	UINT64 rowSizesInBytes;
	UINT64 requiredSize;
	UINT numRows;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	auto textureDesc = texture->DxResource()->GetDesc();
	device->DxDevice()->GetCopyableFootprints(&textureDesc, 0, 1, 0,
											  &footprint, &numRows, &rowSizesInBytes, &requiredSize);

	PNuoResource intermediateResource = device->CreateUploadBuffer(requiredSize);
	intermediate.push_back(intermediateResource);
	
	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = src.data();
	textureData.RowPitch = (UINT32)texture->Width() * 4;
	textureData.SlicePitch = textureData.RowPitch * texture->Height();

	ResourceBarrier(texture, D3D12_RESOURCE_STATE_COPY_DEST);
	UpdateSubresources(_commandList.Get(), texture->DxResource(), intermediateResource->DxResource(), 0, 0, 1, &textureData);
	ResourceBarrier(texture, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
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


void NuoCommandBuffer::WaitUntilComplete(std::vector<PNuoResource>& intermediate)
{
	// must be non-in-flight
	assert(_frameCount == 0);

	PNuoCommandQueue commandQueue = CommandQueue();
	PNuoDevice device = commandQueue->Device();

	PNuoFenceSwapChain fence = device->CreateFenceSwapChain(1);
	fence->WaitForGPU(commandQueue);

	intermediate.clear();
}


PNuoCommandQueue NuoCommandBuffer::CommandQueue() const
{
	return _commandQueue;
}



void NuoCommandEncoder::SetClearColor(const NuoVectorFloat4& color)
{
	float acolor[] = { color.x(), color.y(), color.z(), color.w() };

	for (unsigned int i = 0; i < _renderTarget->AttachmentNumber(); ++i)
		_commandList->ClearRenderTargetView(_renderTarget->View()[i], acolor, 0, nullptr);
}


void NuoCommandEncoder::ClearDepth()
{
	_commandList->ClearDepthStencilView(_renderTarget->DepthView(), D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr);
}


void NuoCommandEncoder::SetViewport(const NuoViewport& viewport)
{
	FLOAT w = (FLOAT)_renderTarget->Width();
	FLOAT h = (FLOAT)_renderTarget->Height();

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


void NuoCommandEncoder::SetRootConstantBuffer(unsigned int index, const PNuoResource& buffer)
{
	const D3D12_GPU_VIRTUAL_ADDRESS addr = buffer->DxResource()->GetGPUVirtualAddress();
	_commandList->SetGraphicsRootConstantBufferView(index, addr);
}


void NuoCommandEncoder::SetDescriptorTable(unsigned int index, const PNuoDescriptorHeap& table)
{
	ID3D12DescriptorHeap* pHeaps[] = { table->DxHeap() };

	_commandList->SetDescriptorHeaps(_countof(pHeaps), pHeaps);
	_commandList->SetGraphicsRootDescriptorTable(index, table->DxHeapGPUHandle());
}


void NuoCommandEncoder::SetRenderTarget(const PNuoRenderTarget& renderTarget)
{
	_renderTarget = renderTarget;

	auto renderTargetHandle = _renderTarget->View();
	auto depthView = _renderTarget->DepthView();

	_commandList->OMSetRenderTargets(_renderTarget->AttachmentNumber(), renderTargetHandle, false, &depthView);
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
									    D3D12_RESOURCE_STATES state)
{
	if (resource->State() == state)
	{
		return;
	}

	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource->DxResource();
	barrier.Transition.StateBefore = resource->State();
	barrier.Transition.StateAfter = state;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	resource->SetState(state);

	_commandList->ResourceBarrier(1, &barrier);
}


