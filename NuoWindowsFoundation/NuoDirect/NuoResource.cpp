

#include "NuoResource.h"



NuoResource::NuoResource()
	: _mapped(nullptr)
{
}

NuoResource::~NuoResource()
{
	Unmap();
}


void NuoResource::SetResource(Microsoft::WRL::ComPtr<ID3D12Resource> resource)
{
	_dxResources = resource;
	_desc = _dxResources->GetDesc();
}


unsigned long NuoResource::Width() const
{
	return (unsigned long)_desc.Width;
}


unsigned long NuoResource::Height() const
{
	return (unsigned long)_desc.Height;
}

unsigned long NuoResource::Size() const
{
	return (unsigned long)(_desc.Width * _desc.Height);
}


DXGI_FORMAT NuoResource::Format() const
{
	return _desc.Format;
}


unsigned int NuoResource::SampleCount() const
{
	return _desc.SampleDesc.Count;
}


void* NuoResource::Map()
{
	if (!_mapped)
	{
		D3D12_RANGE readRange = { 0, 0 };
		_dxResources->Map(0, &readRange, &_mapped);
	}

	return _mapped;
}


void NuoResource::Unmap()
{
	if (!_mapped)
		return;

	_dxResources->Unmap(0, nullptr);
	_mapped = nullptr;
}


ID3D12Resource* NuoResource::DxResource() const
{
	return _dxResources.Get();
}
