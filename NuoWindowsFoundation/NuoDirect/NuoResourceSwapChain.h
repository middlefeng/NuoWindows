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


class NuoResourceSwapChain
{

	std::vector<PNuoResource> _resources;

public:

	NuoResourceSwapChain(unsigned int frameCount);
	NuoResourceSwapChain(const std::vector<PNuoResource>& resources);
	
	PNuoResource operator [] (unsigned int inFlight);

};

