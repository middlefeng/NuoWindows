

#include "NuoResourceSwapChain.h"

#include "NuoDevice.h"
#include "NuoCommandBuffer.h"



NuoResourceSwapChain::NuoResourceSwapChain(const PNuoDevice& device, unsigned int frameCount, unsigned long size)
	: _size(size)
{
	_resources.resize(frameCount);

	for (PNuoResource& resource : _resources)
		resource = device->CreateUploadBuffer(size);
}


NuoResourceSwapChain::NuoResourceSwapChain(const std::vector<PNuoResource>& resources)
{
	_resources.insert(_resources.begin(), resources.begin(), resources.end());
}


PNuoResource NuoResourceSwapChain::operator [] (unsigned int inFlight)
{
	return _resources[inFlight];
}

unsigned int NuoResourceSwapChain::Count() const
{
	return (unsigned int)_resources.size();
}


void NuoResourceSwapChain::UpdateResource(void* data, unsigned long size, unsigned int inFlight)
{
	void* dst = _resources[inFlight]->Map();
	memcpy(dst, data, size);
}


void NuoResourceSwapChain::UpdateResource(void* data, unsigned long size, const PNuoRenderInFlight& inFlight)
{
	UpdateResource(data, size, inFlight->InFlight());
}


D3D12_GPU_VIRTUAL_ADDRESS NuoResourceSwapChain::GPUAddress(unsigned int inFlight)
{
	return _resources[inFlight]->DxResource()->GetGPUVirtualAddress();
}
