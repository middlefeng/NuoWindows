

#include "NuoResourceSwapChain.h"



NuoResourceSwapChain::NuoResourceSwapChain(unsigned int frameCount)
{
	_resources.resize(frameCount);
}


NuoResourceSwapChain::NuoResourceSwapChain(const std::vector<PNuoResource>& resources)
{
	_resources.insert(_resources.begin(), resources.begin(), resources.end());
}


PNuoResource NuoResourceSwapChain::operator [] (unsigned int inFlight)
{
	return _resources[inFlight];
}

