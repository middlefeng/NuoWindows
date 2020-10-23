#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class NuoResource;
typedef std::shared_ptr<NuoResource> PNuoResource;
typedef std::weak_ptr<NuoResource> WPNuoResource;


class NuoResource
{

	Microsoft::WRL::ComPtr<ID3D12Resource> _dxResources;

public:

	NuoResource();
	ID3D12Resource* DxResource() const;

	friend class NuoSwapChain;

};

