#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoDevice.h"
#include "NuoResourceSwapChain.h"
#include "NuoDescriptorHeap.h"


class NuoRenderTargetSwapChain;
typedef std::shared_ptr<NuoRenderTargetSwapChain> PNuoRenderTargetSwapChain;
typedef std::weak_ptr<NuoRenderTargetSwapChain> WPNuoRenderTargetSwapChain;



class NuoRenderTargetSwapChain
{

	PNuoDescriptorHeap _rtvHeap;

	PNuoResourceSwapChain _resources;
	WPNuoDevice _device;

public:

	NuoRenderTargetSwapChain(const PNuoDevice& device,
							 const PNuoResourceSwapChain& renderTargets);
	
private:

	D3D12_CPU_DESCRIPTOR_HANDLE DxRenderTargetView(unsigned int inFlight);

	friend class NuoDirectView;
};

