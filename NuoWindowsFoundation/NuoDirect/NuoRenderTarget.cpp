

#include "NuoRenderTarget.h"
#include "NuoDevice.h"
#include "NuoTexture.h"


#include <cassert>


NuoRenderTarget::NuoRenderTarget(const PNuoDevice& device,
								 unsigned int width, unsigned int height,
	                             unsigned int sampleCount, bool depthEnabled)
	: _encoderCount(0),
	  _width(width), _height(height),
	  _sampleCount(sampleCount)
{
	if (depthEnabled)
	{
		CreateDepth(device);
	}
}


NuoRenderTarget::NuoRenderTarget(const PNuoDevice& device, DXGI_FORMAT format,
								 unsigned int width, unsigned int height,
								 unsigned int sampleCount, bool depthEnabled, bool manageResource)
	: _encoderCount(0),
	  _width(width), _height(height),
	  _sampleCount(sampleCount)
{
	if (depthEnabled)
	{
		CreateDepth(device);
	}

	if (sampleCount > 1)
		_sampleResource = device->CreateTexture(format, width, height, sampleCount);

	if (manageResource)
		_resource = device->CreateTexture(format, width, height, 1);


	if (sampleCount > 1 || manageResource)
	{
		PNuoResource resource = _sampleResource ? _sampleResource : _resource;

		_rtvHeap = device->CreateRenderTargetHeap(1);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = _rtvHeap->DxHeapCPUHandle();
		device->DxDevice()->CreateRenderTargetView(resource->DxResource(), nullptr, rtvHandle);

		_view = _rtvHeap->DxHeapCPUHandle();
	}
}


void NuoRenderTarget::CreateDepth(const PNuoDevice& device)
{
	_dsvHeap = device->CreateDepthStencilHeap();
	_depthResource = device->CreateDepthStencil(_width, _height, _sampleCount);

	D3D12_DEPTH_STENCIL_VIEW_DESC dsv = {};
	dsv.Format = DXGI_FORMAT_D32_FLOAT;
	dsv.ViewDimension = _sampleCount > 1 ? D3D12_DSV_DIMENSION_TEXTURE2DMS : D3D12_DSV_DIMENSION_TEXTURE2D;
	dsv.Texture2D.MipSlice = 0;
	dsv.Flags = D3D12_DSV_FLAG_NONE;

	device->DxDevice()->CreateDepthStencilView(_depthResource->DxResource(), &dsv, _dsvHeap->DxHeapCPUHandle());

	_depthView = _dsvHeap->DxHeapCPUHandle();
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoRenderTarget::View()
{
	return _view;
}


unsigned int NuoRenderTarget::AttachmentNumber() const
{
	return 1;
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoRenderTarget::DepthView()
{
	return _depthView;
}


D3D12_GPU_VIRTUAL_ADDRESS NuoRenderTarget::TargetGPUAddress()
{
	// back buffer must not be used as shader resource
	//
	assert(_backBuffer == nullptr);

	return _resource->DxResource()->GetGPUVirtualAddress();
}


PNuoResource NuoRenderTarget::RenderBuffer() const
{
	if (_sampleResource)
		return _sampleResource;

	if (_resource)
		return _resource;

	return _backBuffer;
}


PNuoTexture NuoRenderTarget::ResultTexture() const
{
	return _resource;
}


PNuoTexture NuoRenderTarget::ResultTexture(unsigned int index) const
{
	assert(index == 1);

	return _resource;
}


void NuoRenderTarget::SetBackBuffer(const PNuoResource& backBuffer,
									const D3D12_CPU_DESCRIPTOR_HANDLE& view)
{
	_backBuffer = backBuffer;

	if (!_sampleResource)
		_view = view;
}


PNuoCommandEncoder NuoRenderTarget::RetainRenderPassEncoder(const PNuoCommandBuffer& commandBuffer)
{
	if (_renderPassEncoder)
	{
		_encoderCount += 1;
		return _renderPassEncoder;
	}

	PNuoCommandEncoder encoder = commandBuffer->CreateRenderPassEncoder();
	encoder->SetRenderTarget(shared_from_this());

	encoder->ResourceBarrier(RenderBuffer(),
							 D3D12_RESOURCE_STATE_RENDER_TARGET);

	_encoderCount += 1;
	_renderPassEncoder = encoder;

	return encoder;
}



void NuoRenderTarget::ReleaseRenderPassEncoder()
{
	_encoderCount -= 1;
	if (_encoderCount == 0)
	{
		if (_sampleResource)
		{
			_renderPassEncoder->ResourceBarrier(RenderBuffer(), D3D12_RESOURCE_STATE_RESOLVE_SOURCE);

			PNuoResource presentResource = _resource ? _resource : _backBuffer;

			_renderPassEncoder->ResourceBarrier(presentResource, D3D12_RESOURCE_STATE_RESOLVE_DEST);

			_renderPassEncoder->DxEncoder()->ResolveSubresource(presentResource->DxResource(), 0,
																_sampleResource->DxResource(), 0,
																presentResource->Format());

			_renderPassEncoder->ResourceBarrier(presentResource, D3D12_RESOURCE_STATE_PRESENT);
		}
		else
		{
			_renderPassEncoder->ResourceBarrier(RenderBuffer(), D3D12_RESOURCE_STATE_PRESENT);
		}

		_renderPassEncoder->EndEncoding();
		_renderPassEncoder.reset();
	}
}


DXGI_FORMAT NuoRenderTarget::Format() const
{
	return RenderBuffer()->Format();
}


unsigned int NuoRenderTarget::SampleCount() const
{
	return _sampleCount;
}


unsigned int NuoRenderTarget::Width() const
{
	return _width;
}



unsigned int NuoRenderTarget::Height() const
{
	return _height;
}


