

#include "NuoRenderTarget.h"


#include <cassert>


NuoRenderTarget::NuoRenderTarget()
	: _encoderCount(0)
{
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoRenderTarget::View()
{
	return _view;
}


PNuoResource NuoRenderTarget::Resource()
{
	return _resource;
}

