#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoResource.h"


class NuoDescriptorHeap;
typedef std::shared_ptr<NuoDescriptorHeap> PNuoDescriptorHeap;


class NuoTexture : public NuoResource
{

	PNuoDescriptorHeap _srvHeap;

public:

	NuoTexture();
	virtual ~NuoTexture();

	D3D12_CPU_DESCRIPTOR_HANDLE ShaderResourceView();

	friend class NuoSwapChain;
	friend class NuoDevice;

};

