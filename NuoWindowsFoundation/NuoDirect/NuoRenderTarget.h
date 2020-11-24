#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoDirect/NuoResource.h"
#include "NuoDirect/NuoCommandBuffer.h"



class NuoRenderTarget;
typedef std::shared_ptr<NuoRenderTarget> PNuoRenderTarget;
typedef std::weak_ptr<NuoRenderTarget> WPNuoRenderTarget;

class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;



class NuoRenderTarget : public std::enable_shared_from_this<NuoRenderTarget>
{

	PNuoTexture _resource;
	PNuoTexture _sampleResource;
	PNuoResource _backBuffer;

	D3D12_CPU_DESCRIPTOR_HANDLE _view;
	PNuoDescriptorHeap _rtvHeap;

	PNuoResource _depthResource;
	D3D12_CPU_DESCRIPTOR_HANDLE _depthView;
	PNuoDescriptorHeap _dsvHeap;

	unsigned int _encoderCount;
	PNuoCommandEncoder _renderPassEncoder;

public:

	NuoRenderTarget(const PNuoDevice& device, DXGI_FORMAT format,
					unsigned int width, unsigned int height,
					unsigned int sampleCount, bool depthEnabled, bool manageResource);

	D3D12_CPU_DESCRIPTOR_HANDLE View();
	D3D12_CPU_DESCRIPTOR_HANDLE DepthView();
	D3D12_GPU_DESCRIPTOR_HANDLE ShaderResource();
	PNuoResource RenderBuffer() const;

	void SetBackBuffer(const PNuoResource& backBuffer,
					   const D3D12_CPU_DESCRIPTOR_HANDLE& view);

	PNuoCommandEncoder RetainRenderPassEncoder(const PNuoCommandBuffer& commandBuffer);
	void ReleaseRenderPassEncoder();

	DXGI_FORMAT Format() const;
	unsigned int SampleCount() const;

};




