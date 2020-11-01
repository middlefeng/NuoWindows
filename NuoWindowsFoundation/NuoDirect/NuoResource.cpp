

#include "NuoResource.h"



NuoResource::NuoResource()
{
}


void NuoResource::SetResource(Microsoft::WRL::ComPtr<ID3D12Resource> resource)
{
	_dxResources = resource;
	_desc = _dxResources->GetDesc();
}


unsigned long NuoResource::Width() const
{
	return _desc.Width;
}


unsigned long NuoResource::Height() const
{
	return _desc.Height;
}

size_t NuoResource::Size() const
{
	return _desc.Width * _desc.Height;
}


ID3D12Resource* NuoResource::DxResource() const
{
	return _dxResources.Get();
}
