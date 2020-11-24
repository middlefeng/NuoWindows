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
	D3D12_RESOURCE_DESC _desc;
	void* _mapped;

	void SetResource(Microsoft::WRL::ComPtr<ID3D12Resource> resource);

public:

	NuoResource();
	~NuoResource();
	ID3D12Resource* DxResource() const;

	unsigned long Width() const;
	unsigned long Height() const;
	unsigned long Size() const;

	DXGI_FORMAT Format() const;
	unsigned int SampleCount() const;

	void* Map();
	void Unmap();

	friend class NuoSwapChain;
	friend class NuoDevice;

};

