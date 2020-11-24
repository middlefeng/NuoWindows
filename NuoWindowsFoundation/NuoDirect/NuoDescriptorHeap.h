#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class NuoDevice;


class NuoDescriptorHeap;
typedef std::shared_ptr<NuoDescriptorHeap> PNuoDescriptorHeap;
typedef std::weak_ptr<NuoDescriptorHeap> WPNuoDescriptorHeap;


class NuoDescriptorHeap
{	

	std::weak_ptr<NuoDevice> _device;
	
	unsigned int _size;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _heap;

private:

	NuoDescriptorHeap() : _size(0) {};

public:

	// CPU descriptor is used for both handle and view for render targets
	//
	D3D12_CPU_DESCRIPTOR_HANDLE DxRenderTargetView(unsigned int inFlight);

	D3D12_CPU_DESCRIPTOR_HANDLE DxConstantBufferHandle(unsigned int inFlight);
	D3D12_CPU_DESCRIPTOR_HANDLE DxHeapCPUHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE DxHeapGPUHandle();

	friend class NuoDevice;

};

