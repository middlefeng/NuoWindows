#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoDirect/NuoDescriptorHeap.h"
#include "NuoDirect/NuoFenceSwapChain.h"


class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;
typedef std::weak_ptr<NuoDevice> WPNuoDevice;

class NuoResource;
typedef std::shared_ptr<NuoResource> PNuoResource;

class NuoTexture;
typedef std::shared_ptr<NuoTexture> PNuoTexture;


class NuoDevice : public std::enable_shared_from_this<NuoDevice>
{

	Microsoft::WRL::ComPtr<ID3D12Device8> _dxDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory6> _dxFactor;
	DXGI_ADAPTER_DESC1 _dxDesc;

	static std::set<PNuoDevice> _devices;

public:

	static std::set<PNuoDevice> Devices();

	void EnableDebugInfoQueue();

	std::string Name() const;
	bool SupportRayTracing();

	unsigned int RenderTargetDescriptorHandleIncrementSize() const;
	PNuoDescriptorHeap CreateRenderTargetHeap(unsigned int frameCount);

	unsigned int ShaderResourceDescriptorHandleIncrementSize() const;
	PNuoDescriptorHeap CreateShaderDescriptorHeap(unsigned int frameCount);

	PNuoDescriptorHeap CreateDepthStencilHeap();

	PNuoFenceSwapChain CreateFenceSwapChain(unsigned int frameCount);
	PNuoResource CreateBuffer(void* data, size_t size);
	PNuoResource CreatePrivateBuffer(size_t size);
	PNuoResource CreateUploadBuffer(size_t size);
	PNuoResource CreateDepthStencil(size_t width, size_t height, unsigned int sampleCount);

	PNuoTexture CreateTexture(DXGI_FORMAT format,
							  unsigned int width, unsigned int height, unsigned int sampleCount);

	ID3D12Device* DxDevice() const;
	IDXGIFactory6* DxFactory() const;

private:

	PNuoResource CreateBufferInternal(void* data,
									  size_t width, size_t height,
									  D3D12_RESOURCE_DIMENSION dimension,
									  DXGI_FORMAT format,
									  unsigned int sampleCount,
									  D3D12_TEXTURE_LAYOUT layout,
									  D3D12_RESOURCE_STATES state,
									  D3D12_RESOURCE_FLAGS flags);

};


