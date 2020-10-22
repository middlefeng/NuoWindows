#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;


class NuoDevice
{

	Microsoft::WRL::ComPtr<ID3D12Device> _dxDevice;
	DXGI_ADAPTER_DESC1 _dxDesc;

	static std::set<PNuoDevice> _devices;

public:

	static std::set<PNuoDevice> Devices();

	std::string Name();

};


