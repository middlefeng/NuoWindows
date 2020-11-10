#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoResource.h"


class NuoConstantBufferSwapChain;
typedef std::shared_ptr<NuoConstantBufferSwapChain> PNuoConstantBufferSwapChain;
typedef std::weak_ptr<NuoConstantBufferSwapChain> WPNuoConstantBufferSwapChain;

class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;

class NuoResourceSwapChain;
typedef std::shared_ptr<NuoResourceSwapChain> PNuoResourceSwapChain;

class NuoDescriptorHeap;
typedef std::shared_ptr<NuoDescriptorHeap> PNuoDescriptorHeap;



class NuoConstantBufferSwapChain
{

	PNuoResourceSwapChain _resources;
	PNuoDescriptorHeap _cbvHeap;

public:

	NuoConstantBufferSwapChain(const PNuoDevice& device,
							   unsigned int frameCount, unsigned int size);

	void UpdateBuffer(void* data, unsigned long size, unsigned int inFlight);
	D3D12_GPU_VIRTUAL_ADDRESS Buffer(unsigned int inFlight);

};

