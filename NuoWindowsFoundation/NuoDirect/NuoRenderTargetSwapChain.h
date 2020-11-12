#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoDevice.h"
#include "NuoResourceSwapChain.h"
#include "NuoRenderTarget.h"
#include "NuoDescriptorHeap.h"


class NuoRenderTargetSwapChain;
typedef std::shared_ptr<NuoRenderTargetSwapChain> PNuoRenderTargetSwapChain;
typedef std::weak_ptr<NuoRenderTargetSwapChain> WPNuoRenderTargetSwapChain;



class NuoRenderTargetSwapChain
{

	PNuoDescriptorHeap _rtvHeap;
	PNuoDescriptorHeap _dsvHeap;

	PNuoResource _depthStencil;
	PNuoResourceSwapChain _resources;
	PNuoResourceSwapChain _sampleResources;
	unsigned int _sampleCount;
	WPNuoDevice _device;

	std::vector<PNuoRenderTarget> _renderTargets;

public:

	NuoRenderTargetSwapChain(const PNuoDevice& device,
							 const PNuoResourceSwapChain& renderTargets,
							 unsigned int sampleCount);
	
	PNuoRenderTarget RenderTarget(unsigned int inFlight);
	
private:

	D3D12_CPU_DESCRIPTOR_HANDLE DxRenderTargetView(unsigned int inFlight);
	D3D12_CPU_DESCRIPTOR_HANDLE DxDepthStencilView();

	void CreateSampleResources();

	friend class NuoDirectView;
};

