#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoDirect/NuoDevice.h"
#include "NuoDirect/NuoPipelineState.h"
#include "NuoDirect/NuoCommandQueue.h"
#include "NuoDirect/NuoVertexBuffer.h"

#include "NuoMathVector.h"

class NuoCommandSwapChain;
typedef std::shared_ptr<NuoCommandSwapChain> PNuoCommandSwapChain;

class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;

class NuoCommandEncoder;
typedef std::shared_ptr<NuoCommandEncoder> PNuoCommandEncoder;


class NuoCommandSwapChain
{

	PNuoCommandQueue _commandQueue;

	std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> _commandAllocators;

	// to retain resources that GPU need in execution.
	//
	std::vector<std::vector<PNuoCommandBuffer>> _inFlightBuffers;

public:

	NuoCommandSwapChain(const PNuoCommandQueue& commandQueue, unsigned int frameCount);

	PNuoCommandQueue CommandQueue() const;
	PNuoCommandBuffer CreateCommandBuffer(unsigned int inFlight, bool resetAllocator);

};


class NuoRenderInFlight
{
protected:
	unsigned int _inFlight;
	unsigned int _frameCount;

public:
	virtual unsigned int InFlight() const;
	virtual unsigned int FrameCount() const;

	void SetInFlight(unsigned int inFlight, unsigned int frameCount);
};


class NuoCommandBuffer : public NuoRenderInFlight,
						 public std::enable_shared_from_this<NuoCommandBuffer>
{
	PNuoCommandQueue _commandQueue;
	std::vector<PNuoCommandEncoder> _encoders;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _commandAllocator;

public:

	NuoCommandBuffer() = default;
	PNuoCommandEncoder CreateRenderPassEncoder();
	void CopyResource(const PNuoResource& src, const PNuoResource& dst);
	void Commit();

	PNuoCommandQueue CommandQueue() const;

	friend class NuoCommandSwapChain;
	friend class NuoCommandQueue;
};


class NuoRenderTarget;
typedef std::shared_ptr<NuoRenderTarget> PNuoRenderTarget;

class NuoResourceSwapChain;
typedef std::shared_ptr<NuoResourceSwapChain> PNuoResourceSwapChain;



class NuoViewport
{
public:

	D3D12_VIEWPORT _viewport;

	NuoViewport();

	NuoViewport(float topLeftX, float topLeftY,
				float width, float height,
				float minDepth, float maxDepth);
};



class NuoCommandEncoder : public NuoRenderInFlight
{
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _commandList;

	PNuoRenderTarget _renderTarget;
	PNuoCommandQueue _commandQueue;

public:

	void SetClearColor(const NuoVectorFloat4& color);
	void ClearDepth();
	void SetViewport(const NuoViewport& viewport);

	void SetRootConstant(unsigned int index, size_t size, void* constant);
	void SetRootConstantBuffer(unsigned int index, const PNuoResourceSwapChain& cb);
	void SetRootConstantBuffer(unsigned int index, const PNuoResource& buffer);
	void SetDescriptorTable(unsigned int index, const PNuoDescriptorHeap& table);
	
	void SetRenderTarget(const PNuoRenderTarget& renderTarget);

	void SetPipeline(const PNuoPipelineState& pipeline);
	void SetVertexBuffer(const PNuoVertexBuffer& vertexBuffer);
	void DrawIndexed(unsigned int indiciesCount);
	void CopyResource(const PNuoResource& src, const PNuoResource& dst);
	void CopyTexture(const std::vector<UINT8>& src, std::vector<PNuoResource>& intermediate, const PNuoTexture& texture);
	void EndEncoding();

private:

	void ResourceBarrier(const PNuoResource& resource,
						 D3D12_RESOURCE_STATES state);

	ID3D12GraphicsCommandList* DxEncoder();

	friend class NuoCommandBuffer;
	friend class NuoRenderTarget;
	friend class NuoRenderPassAttachment;
};


