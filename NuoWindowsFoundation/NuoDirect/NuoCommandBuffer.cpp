

#include "NuoCommandBuffer.h"

#include <windows.h>
#include <cassert>



NuoCommandSwapChain::NuoCommandSwapChain(const PNuoDevice& device, unsigned int frameCount)
	: _device(device)
{
	_commandAllocators.resize(frameCount);
	_commandBuffers.resize(frameCount);

	for (UINT n = 0; n < frameCount; n++)
	{
		HRESULT hr = device->DxDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&_commandAllocators[n]));
		assert(hr == S_OK);
	}
}

PNuoDevice NuoCommandSwapChain::Device() const
{
	return _device;
}
