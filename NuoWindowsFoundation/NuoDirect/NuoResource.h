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


class NuoResource : public std::enable_shared_from_this<NuoResource>
{

protected:

	Microsoft::WRL::ComPtr<ID3D12Resource> _dxResources;
	D3D12_RESOURCE_DESC _desc;
	D3D12_RESOURCE_STATES _state;
	void* _mapped;

	virtual void SetResource(Microsoft::WRL::ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES state);

public:

	NuoResource();
	virtual ~NuoResource();
	ID3D12Resource* DxResource() const;

	unsigned long Width() const;
	unsigned long Height() const;
	unsigned long Size() const;

	DXGI_FORMAT Format() const;
	unsigned int SampleCount() const;

	void* Map();
	void Unmap();

	D3D12_RESOURCE_STATES State() const;
	void SetState(D3D12_RESOURCE_STATES state);

	friend class NuoSwapChain;
	friend class NuoDevice;

};

