#pragma once

#include <memory>
#include <set>

#include <d3d12.h>
#include <wrl.h>

class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;


class NuoDevice
{

	Microsoft::WRL::ComPtr<ID3D12Device> _dxDevice;

public:

	static std::set<PNuoDevice> Devices();

};


