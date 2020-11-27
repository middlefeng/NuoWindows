#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoResource.h"


class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;



class NuoTexture : public NuoResource
{

public:

	NuoTexture();
	virtual ~NuoTexture();

	void Upload(const PNuoCommandBuffer& commandBuffe, const std::vector<UINT8>& data, std::vector<PNuoResource> intermediate);

	friend class NuoSwapChain;
	friend class NuoDevice;

};

