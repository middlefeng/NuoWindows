

#include "NuoConstantBufferSwapChain.h"

#include "NuoDevice.h"
#include "NuoResourceSwapChain.h"



NuoConstantBufferSwapChain::NuoConstantBufferSwapChain(const PNuoDevice& device,
													   unsigned int frameCount, unsigned int size)
{
	std::vector<PNuoResource> resources;
	resources.resize(size);

	for (PNuoResource& resource : resources)
		resource = device->CreateUploadBuffer(size);

	_resources = std::make_shared<NuoResourceSwapChain>(resources);

	_cbvHeap = device->CreateConstantBufferHeap(frameCount);
	for (UINT n = 0; n < frameCount; ++n)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = _cbvHeap->DxConstantBufferHandle(n);

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
		PNuoResource resource = resources[n];
		desc.BufferLocation = resource->DxResource()->GetGPUVirtualAddress();
		desc.SizeInBytes = resource->Size();

		device->DxDevice()->CreateConstantBufferView(&desc, cbvHandle);
	}
}


void NuoConstantBufferSwapChain::UpdateBuffer(void* data, unsigned long size, unsigned int inFlight)
{
	_resources->UpdateResource(data, size, inFlight);
}


D3D12_GPU_VIRTUAL_ADDRESS NuoConstantBufferSwapChain::Buffer(unsigned int inFlight)
{
	return (*_resources)[inFlight]->DxResource()->GetGPUVirtualAddress();
}


