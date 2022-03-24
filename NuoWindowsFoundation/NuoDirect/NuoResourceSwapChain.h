#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoResource.h"


class NuoResourceSwapChain;
typedef std::shared_ptr<NuoResourceSwapChain> PNuoResourceSwapChain;
typedef std::weak_ptr<NuoResourceSwapChain> WPNuoResourceSwapChain;

class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;

class NuoRenderInFlight;
typedef std::shared_ptr <NuoRenderInFlight> PNuoRenderInFlight;


class NuoResourceSwapChain
{

	std::vector<PNuoResource> _resources;
	unsigned long _size;

public:

	NuoResourceSwapChain(const PNuoDevice& device, unsigned int frameCount, unsigned long size);
	NuoResourceSwapChain(const std::vector<PNuoResource>& resources);
	
	PNuoResource operator [] (unsigned int inFlight);
	unsigned int Count() const;

	void UpdateResource(void* data, unsigned long size, unsigned int inFlight);
	void UpdateResource(void* data, unsigned long size, const PNuoRenderInFlight& inFlight);
	D3D12_GPU_VIRTUAL_ADDRESS GPUAddress(unsigned int inFlight);

};

