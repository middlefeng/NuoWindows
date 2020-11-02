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

public:

	NuoCommandSwapChain(const PNuoCommandQueue& commandQueue, unsigned int frameCount);

	PNuoCommandQueue CommandQueue() const;
	PNuoCommandBuffer CreateCommandBuffer(unsigned int inFlight, bool resetAllocator);

};


class NuoRenderInFlight
{
protected:
	unsigned int _inFlight;

public:
	virtual unsigned int InFlight();
};


class NuoCommandBuffer : public NuoRenderInFlight
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



class NuoCommandEncoder : public NuoRenderInFlight
{
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _commandAllocator;
	std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>> _commandList;

	PNuoCommandQueue _commandQueue;
	PNuoRenderTarget _renderTarget;

public:

	void SetConstant(unsigned int index, size_t size, void* constant);

	void SetPipeline(const PNuoPipelineState& pipeline);
	void ClearTargetView(float r, float g, float b, float a);
	void SetVertexBuffer(const PNuoVertexBuffer& vertexBuffer);
	void DrawInstanced(unsigned int vertexCount, unsigned int instance);
	void UseDefaultViewPort();
	void CopyResource(const PNuoResource& src, const PNuoResource& dst);
	void EndEncoding();

private:

	void Commit();
	void ResourceBarrier(const PNuoResource& resource,
						 D3D12_RESOURCE_STATES before,
						 D3D12_RESOURCE_STATES after);

	friend class NuoCommandBuffer;
	friend class NuoRenderTarget;
};


