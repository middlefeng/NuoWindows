#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoDirect/NuoDescriptorHeap.h"
#include "NuoDirect/NuoFenceSwapChain.h"


class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;
typedef std::weak_ptr<NuoDevice> WPNuoDevice;


class NuoDevice : public std::enable_shared_from_this<NuoDevice>
{

	Microsoft::WRL::ComPtr<ID3D12Device> _dxDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory6> _dxFactor;
	DXGI_ADAPTER_DESC1 _dxDesc;

	static std::set<PNuoDevice> _devices;

public:

	static std::set<PNuoDevice> Devices();

	void EnableDebugInfoQueue();

	std::string Name() const;
	unsigned int RenderTargetDescriptorHandleIncrementSize() const;
	PNuoDescriptorHeap CreateRenderTargetHeap(unsigned int frameCount);

	PNuoFenceSwapChain CreateFenceSwapChain(unsigned int frameCount);

	ID3D12Device* DxDevice() const;
	IDXGIFactory6* DxFactory() const;

};


