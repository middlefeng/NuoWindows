

#include "NuoResource.h"



NuoResource::NuoResource()
{
}


ID3D12Resource* NuoResource::DxResource() const
{
	return _dxResources.Get();
}
