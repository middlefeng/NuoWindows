#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoDirect/NuoDevice.h"

class NuoCommandQueue;
typedef std::shared_ptr<NuoCommandQueue> PNuoCommandQueue;

class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;


class NuoCommandQueue : public std::enable_shared_from_this<NuoCommandQueue>
{

	PNuoDevice _device;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> _dxQueue;
	std::vector<PNuoCommandBuffer> _pendingCommandBuffers;

public:

	NuoCommandQueue(const PNuoDevice& device);
	PNuoCommandBuffer CreateCommandBuffer();

	PNuoDevice Device() const;
	void ReleasePendingCommandBuffers();

	ID3D12CommandQueue* DxQueue() const;

};


