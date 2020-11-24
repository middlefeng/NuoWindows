

#include "NuoTexture.h"
#include "NuoDescriptorHeap.h"


NuoTexture::NuoTexture()
{
}


NuoTexture::~NuoTexture()
{
}


D3D12_CPU_DESCRIPTOR_HANDLE NuoTexture::ShaderResourceView()
{
	return _srvHeap->DxHeapCPUHandle();
}
