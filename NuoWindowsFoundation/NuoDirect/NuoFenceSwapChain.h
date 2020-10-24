#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>


class NuoDirectView;
typedef std::shared_ptr<NuoDirectView> PNuoDirectView;


class NuoFenceSwapChain;
typedef std::shared_ptr<NuoFenceSwapChain> PNuoFenceSwapChain;
typedef std::weak_ptr<NuoFenceSwapChain> WPNuoFenceSwapChain;



class NuoFenceSwapChain
{

	Microsoft::WRL::ComPtr<ID3D12Fence> _fence;
	std::vector<UINT64> _fenceValues;

	HANDLE _fenceEvent;

public:

	NuoFenceSwapChain(unsigned int frameCount);
	~NuoFenceSwapChain();

	void WaitForGPU(const PNuoDirectView& view);
	void MoveToNextFrame(const PNuoDirectView& view);

	friend class NuoDevice;
};

