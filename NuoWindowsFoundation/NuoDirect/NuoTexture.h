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

public:

	NuoTexture();
	virtual ~NuoTexture();

	friend class NuoSwapChain;
	friend class NuoDevice;

};

