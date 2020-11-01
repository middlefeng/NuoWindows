#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoDirect/NuoDevice.h"
#include "NuoDirect/NuoResource.h"
#include "NuoDirect/NuoCommandBuffer.h"



class NuoRenderTarget;
typedef std::shared_ptr<NuoRenderTarget> PNuoRenderTarget;
typedef std::weak_ptr<NuoRenderTarget> WPNuoRenderTarget;



class NuoRenderTarget : public std::enable_shared_from_this<NuoRenderTarget>
{

	PNuoResource _resource;
	D3D12_CPU_DESCRIPTOR_HANDLE _view;

	unsigned int _encoderCount;
	PNuoCommandEncoder _renderPassEncoder;

public:

	NuoRenderTarget();

	D3D12_CPU_DESCRIPTOR_HANDLE View();
	PNuoResource Resource();

	PNuoCommandEncoder RetainRenderPassEncoder(const PNuoCommandBuffer& commandBuffer);
	void ReleaseRenderPassEncoder();

	friend class NuoRenderTargetSwapChain;
};


