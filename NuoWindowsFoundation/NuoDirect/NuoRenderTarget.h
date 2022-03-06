#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoResource.h"
#include "NuoCommandBuffer.h"
#include "NuoSize.h"



class NuoRenderTarget;
typedef std::shared_ptr<NuoRenderTarget> PNuoRenderTarget;
typedef std::weak_ptr<NuoRenderTarget> WPNuoRenderTarget;

class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;



class NuoRenderTarget : public std::enable_shared_from_this<NuoRenderTarget>
{

protected:

	PNuoDevice _device;

	unsigned int _width;
	unsigned int _height;
	unsigned int _sampleCount;
	
	DXGI_FORMAT _format;
	bool _manageResource;
	bool _depthEnabled;

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

	// used by subclass only as a subclass would provide its own approach of manage resources.
	// so far only the NuoRenderPassTarget uses this
	//
	NuoRenderTarget(const PNuoDevice& device,
		            unsigned int sampleCount, bool depthEnabled);

public:

	NuoRenderTarget(const PNuoDevice& device, DXGI_FORMAT format,
					unsigned int sampleCount, bool depthEnabled, bool manageResource);

	virtual D3D12_CPU_DESCRIPTOR_HANDLE* View();
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
	void SetSampleCount(unsigned int sampleCount);

	NuoSize DrawableSize() const;
	virtual void SetDrawableSize(const NuoSize& size);

	unsigned int Width() const;
	unsigned int Height() const;

protected:

	void CreateDepth();
	void CreateTextures();

};




