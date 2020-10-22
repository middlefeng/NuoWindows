#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;
typedef std::weak_ptr<NuoDevice> WPNuoDevice;


class NuoDevice
{

	Microsoft::WRL::ComPtr<ID3D12Device> _dxDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory6> _dxFactor;
	DXGI_ADAPTER_DESC1 _dxDesc;

	static std::set<PNuoDevice> _devices;

public:

	static std::set<PNuoDevice> Devices();

	std::string Name();

	ID3D12Device* DxDevice() const;
	IDXGIFactory6* DxFactory() const;

};


