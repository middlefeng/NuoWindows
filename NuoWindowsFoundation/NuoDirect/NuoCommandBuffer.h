#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoDirect/NuoDevice.h"

class NuoCommandSwapChain;
typedef std::shared_ptr<NuoCommandSwapChain> PNuoCommandSwapChain;

class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;


class NuoCommandSwapChain
{

	PNuoDevice _device;

	std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> _commandAllocators;
	std::vector<PNuoCommandBuffer> _commandBuffers;

public:

	NuoCommandSwapChain(const PNuoDevice& device, unsigned int frameCount);

	PNuoDevice Device() const;
	NuoCommandBuffer CreateCommandBuffer(unsigned int inFlight);

};


class NuoCommandBuffer
{

};


