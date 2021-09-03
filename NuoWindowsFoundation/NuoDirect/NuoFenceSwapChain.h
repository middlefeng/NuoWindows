#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>


class NuoCommandQueue;
typedef std::shared_ptr<NuoCommandQueue> PNuoCommandQueue;

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

	// used for multi-in-flight-frame rotation
	//
	UINT64 _currentFenceValue;

public:

	NuoFenceSwapChain(unsigned int frameCount);
	~NuoFenceSwapChain();

	void WaitForGPU(const PNuoDirectView& view);
	void WaitForGPU(const PNuoCommandQueue& commandQueue);

	// present() calls the view to present the current buffer of its swap-chain
	// and puts GPU fence
	//
	// prepareFrame() checks the GPU fence to make sure a frame's supporting buffers
	// are not reused until its previous owner has been presented
	//
	void Present(const PNuoDirectView& view);
	void PrepareFrame(const PNuoDirectView& view);

	friend class NuoDevice;

private:

	void WaitForGPU(const PNuoCommandQueue& commandQueue, unsigned int inFlight);

};

