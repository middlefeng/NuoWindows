

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


D3D12_CPU_DESCRIPTOR_HANDLE NuoRenderTarget::DepthView()
{
	return _depthView;
}


PNuoResource NuoRenderTarget::Resource()
{
	if (_sampleResource)
		return _sampleResource;

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
	encoder->SetRenderTarget(shared_from_this());

	if (_sampleResource)
	{
		encoder->ResourceBarrier(Resource(),
								 D3D12_RESOURCE_STATE_RESOLVE_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
	else
	{
		encoder->ResourceBarrier(Resource(),
								 D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}

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
			_renderPassEncoder->ResourceBarrier(Resource(),
												D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_RESOLVE_SOURCE);
			_renderPassEncoder->ResourceBarrier(_resource,
												D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RESOLVE_DEST);

			_renderPassEncoder->DxEncoder()->ResolveSubresource(_resource->DxResource(), 0,
																_sampleResource->DxResource(), 0, _resource->Format());

			_renderPassEncoder->ResourceBarrier(_resource,
												D3D12_RESOURCE_STATE_RESOLVE_DEST, D3D12_RESOURCE_STATE_PRESENT);
		}
		else
		{
			_renderPassEncoder->ResourceBarrier(Resource(),
												D3D12_RESOURCE_STATE_RENDER_TARGET,
												D3D12_RESOURCE_STATE_PRESENT);
		}

		_renderPassEncoder->EndEncoding();
		_renderPassEncoder.reset();
	}
}





