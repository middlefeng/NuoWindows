﻿

#include "NuoRenderTarget.h"


#include <cassert>


NuoRenderTarget::NuoRenderTarget()
	: _encoderCount(0)
{
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoRenderTarget::View()
{
	return _view;
}


PNuoResource NuoRenderTarget::Resource()
{
	return _resource;
}


PNuoCommandEncoder NuoRenderTarget::RetainRenderPassEncoder(const PNuoCommandBuffer& commandBuffer)
{
	if (_renderPassEncoder)
	{
		_encoderCount += 1;
		return _renderPassEncoder;
	}

	PNuoCommandEncoder encoder = commandBuffer->CreateRenderPassEncoder();
	encoder->_renderTarget = shared_from_this();

	_renderPassEncoder = encoder;

	return encoder;
}



void NuoRenderTarget::ReleaseRenderPassEncoder()
{
	_encoderCount -= 1;
	if (_encoderCount == 0)
	{
		_renderPassEncoder->EndEncoding();
		_renderPassEncoder.reset();
	}
}


