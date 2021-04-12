﻿//
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


void NuoRenderPassAttachment::MakeTexture()
{
	unsigned int sampleCount = _renderTarget->SampleCount();
	unsigned int width = _renderTarget->Width();
	unsigned int height = _renderTarget->Height();

	if (sampleCount > 1)
		_sampleResource = _device->CreateTexture(_format, width, height, sampleCount);

	_resource = _device->CreateTexture(_format, width, height, 1);
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
										 unsigned int width, unsigned int height,
										 unsigned int sampleCount, bool depthEnabled)
	: NuoRenderTarget(device, width, height, sampleCount, depthEnabled),
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


void NuoRenderPassTarget::Init()
{
	const unsigned int attachmentNum = AttachmentNumber();
	_rtvHeap = _device->CreateRenderTargetHeap(attachmentNum);

	for (unsigned int i = 0; i < attachmentNum; ++i)
	{
		PNuoRenderPassAttachment attachment = _attachments[i];
		attachment->MakeTexture();

		PNuoResource resource = attachment->RenderBuffer();
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = _rtvHeap->DxCPUHandle(i);

		_device->DxDevice()->CreateRenderTargetView(resource->DxResource(), nullptr, rtvHandle);
	}

	_view = _rtvHeap->DxHeapCPUHandle();
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



