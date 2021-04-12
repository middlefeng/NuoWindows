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

protected:

	unsigned int _width;
	unsigned int _height;
	unsigned int _sampleCount;

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

	
	NuoRenderTarget(const PNuoDevice& device,
				    unsigned int width, unsigned int height,
		            unsigned int sampleCount, bool depthEnabled);

public:

	NuoRenderTarget(const PNuoDevice& device, DXGI_FORMAT format,
					unsigned int width, unsigned int height,
					unsigned int sampleCount, bool depthEnabled, bool manageResource);

	D3D12_CPU_DESCRIPTOR_HANDLE View();
	D3D12_CPU_DESCRIPTOR_HANDLE DepthView();
	D3D12_GPU_VIRTUAL_ADDRESS TargetGPUAddress();
	PNuoResource RenderBuffer() const;

	virtual PNuoTexture ResultTexture() const;
	virtual PNuoTexture ResultTexture(unsigned int index) const;

	virtual unsigned int AttachmentNumber() const;

	void SetBackBuffer(const PNuoResource& backBuffer,
					   const D3D12_CPU_DESCRIPTOR_HANDLE& view);

	virtual PNuoCommandEncoder RetainRenderPassEncoder(const PNuoCommandBuffer& commandBuffer);
	virtual void ReleaseRenderPassEncoder();

	DXGI_FORMAT Format() const;
	unsigned int SampleCount() const;

	unsigned int Width() const;
	unsigned int Height() const;

private:

	void CreateDepth(const PNuoDevice& device);

};




