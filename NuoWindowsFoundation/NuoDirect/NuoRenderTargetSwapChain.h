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
	// back buffers of the _renderTarget are not managed by _renderTarget itself
	// but by the swap-chain, so as the descriptor heap
	//
	// the heap is used only when _sampleCount is 1. when it is greater than 1,
	// the direct render target is the sample buffer managed within the _renderTarget
	// whose view is also managed from within.
	//
	PNuoDescriptorHeap _rtvHeap;
	PNuoResourceSwapChain _backBuffers;
	WPNuoDevice _device;
	PNuoRenderTarget _renderTarget;

	unsigned int _sampleCount;

public:

	NuoRenderTargetSwapChain(const PNuoDevice& device, DXGI_FORMAT format,
							 const PNuoResourceSwapChain& backBuffers,
							 unsigned int sampleCount);
	
	PNuoRenderTarget RenderTarget(unsigned int inFlight);
	
private:

	// used only when _sampleCount is 1
	//
	D3D12_CPU_DESCRIPTOR_HANDLE DxRenderTargetView(unsigned int inFlight);
};

