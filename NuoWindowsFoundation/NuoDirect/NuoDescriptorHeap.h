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

	NuoDescriptorHeap() {};

public:

	D3D12_CPU_DESCRIPTOR_HANDLE DxRenderTargetView(unsigned int inFlight);

	friend class NuoDevice;

};

