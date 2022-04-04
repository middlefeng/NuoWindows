//
//  NuoRenderPassTarget.cpp
//  ModelViewer
//
//  Created by Dong Feng on 4/1/2021.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#include "NuoRenderPassTarget.h"
#include <cassert>

#include "NuoTexture.h"
#include "NuoCommandBuffer.h"



NuoRenderPassAttachment::NuoRenderPassAttachment(const PNuoDevice& device,
	                                             DXGI_FORMAT format, NuoRenderPassTarget* renderTarget)
	: _device(device),
	  _format(format),
	  _renderTarget(renderTarget)
{
}


void NuoRenderPassAttachment::SetClearColor(const NuoVectorFloat4& color)
{
	_clearColor = color;
}



NuoVectorFloat4 NuoRenderPassAttachment::ClearColor() const
{
	return _clearColor;
}


void NuoRenderPassAttachment::MakeTexture()
{
	unsigned int sampleCount = _renderTarget->SampleCount();
	unsigned int width = _renderTarget->Width();
	unsigned int height = _renderTarget->Height();

	if (sampleCount > 1)
		_sampleResource = _device->CreateTexture(_format, width, height, sampleCount, _clearColor);

	_resource = _device->CreateTexture(_format, width, height, 1, _clearColor);
}


PNuoResource NuoRenderPassAttachment::RenderBuffer()
{
	if (_sampleResource)
		return _sampleResource;

	return _resource;
}


PNuoTexture NuoRenderPassAttachment::Texture()
{
	return _resource;
}


void NuoRenderPassAttachment::ResourceUseBegin(const PNuoCommandEncoder& encoder)
{
	encoder->ResourceBarrier(RenderBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET);
}


void NuoRenderPassAttachment::ResourceUseEnd(const PNuoCommandEncoder& encoder)
{
	if (_sampleResource)
	{
		encoder->ResourceBarrier(RenderBuffer(), D3D12_RESOURCE_STATE_RESOLVE_SOURCE);
		encoder->ResourceBarrier(_resource, D3D12_RESOURCE_STATE_RESOLVE_DEST);

		encoder->DxEncoder()->ResolveSubresource(_resource->DxResource(), 0,
												 _sampleResource->DxResource(), 0,
												 _resource->Format());

		encoder->ResourceBarrier(_resource, D3D12_RESOURCE_STATE_PRESENT);
	}
	else
	{
		encoder->ResourceBarrier(RenderBuffer(), D3D12_RESOURCE_STATE_PRESENT);
	}
}



NuoRenderPassTarget::NuoRenderPassTarget(const PNuoDevice& device,
										 unsigned int sampleCount, bool depthEnabled)
	: NuoRenderTarget(device, DXGI_FORMAT_UNKNOWN /* the format is in the attachment instead */,
					  sampleCount, depthEnabled,
					  true /* manage resource by itself, but not directly (through the attachments instead) */),
	  _device(device)
{
}


void NuoRenderPassTarget::AddAttachment(DXGI_FORMAT format)
{
	PNuoRenderPassAttachment attachment = 
			std::make_shared<NuoRenderPassAttachment>(_device, format, this);
	_attachments.push_back(attachment);
}


unsigned int NuoRenderPassTarget::AttachmentNumber() const
{
	return (unsigned int)_attachments.size();
}


D3D12_CPU_DESCRIPTOR_HANDLE* NuoRenderPassTarget::View()
{
	return _views.data();
}


void NuoRenderPassTarget::Init()
{
	const unsigned int attachmentNum = AttachmentNumber();

	_rtvHeap = _device->CreateRenderTargetHeap(attachmentNum);
	_views.resize(attachmentNum);

	for (unsigned int i = 0; i < attachmentNum; ++i)
	{
		PNuoRenderPassAttachment attachment = _attachments[i];
		attachment->MakeTexture();

		PNuoResource resource = attachment->RenderBuffer();
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = _rtvHeap->DxCPUHandle(i);

		_device->DxDevice()->CreateRenderTargetView(resource->DxResource(), nullptr, rtvHandle);

		_views[i] = rtvHandle;
	}

	_view = _rtvHeap->DxHeapCPUHandle();
}


void NuoRenderPassTarget::SetClearColor(const NuoVectorFloat4& color)
{
	NuoRenderTarget::SetClearColor(color);

	for (PNuoRenderPassAttachment attachment : _attachments)
	{
		attachment->SetClearColor(color);
	}
}


PNuoCommandEncoder NuoRenderPassTarget::RetainRenderPassEncoder(const PNuoCommandBuffer& commandBuffer)
{
	if (_renderPassEncoder)
	{
		_encoderCount += 1;
		return _renderPassEncoder;
	}

	PNuoCommandEncoder encoder = commandBuffer->CreateRenderPassEncoder();
	encoder->SetRenderTarget(shared_from_this());

	for (PNuoRenderPassAttachment attachment : _attachments)
		attachment->ResourceUseBegin(encoder);

	encoder->SetClearColor(_clearColor);

	_encoderCount += 1;
	_renderPassEncoder = encoder;

	return encoder;
}



void NuoRenderPassTarget::ReleaseRenderPassEncoder()
{
	_encoderCount -= 1;
	if (_encoderCount == 0)
	{
		for (PNuoRenderPassAttachment attachment : _attachments)
			attachment->ResourceUseEnd(_renderPassEncoder);

		_renderPassEncoder->EndEncoding();
		_renderPassEncoder.reset();
	}
}


PNuoTexture NuoRenderPassTarget::ResultTexture(unsigned int index) const
{
	auto texture = _attachments[index]->Texture();
	return texture;
}


void NuoRenderPassTarget::SetDrawableSize(const NuoSize& size)
{
	if (_width == size.X() && _height == size.Y())
	{
		return;
	}

	_width = (unsigned int)size.X();
	_height = (unsigned int)size.Y();

	_views.clear();

	Init();

	if (_depthEnabled)
	{
		CreateDepth();
	}
}


