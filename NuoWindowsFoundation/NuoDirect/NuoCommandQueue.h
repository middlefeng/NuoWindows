#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoDirect/NuoDevice.h"

class NuoCommandQueue;
typedef std::shared_ptr<NuoCommandQueue> PNuoCommandQueue;


class NuoCommandQueue
{

	PNuoDevice _device;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> _dxQueue;

public:

	NuoCommandQueue(const PNuoDevice& device);

	PNuoDevice Device() const;

	ID3D12CommandQueue* DxQueue() const;

};


