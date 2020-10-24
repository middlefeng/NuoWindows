#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoView.h"

#include "NuoDirect/NuoDevice.h"
#include "NuoDirect/NuoCommandQueue.h"
#include "NuoDirect/NuoResourceSwapChain.h"
#include "NuoDirect/NuoRenderTargetSwapChain.h"



class NuoDirectView;
typedef std::shared_ptr<NuoDirectView> PNuoDirectView;
typedef std::weak_ptr<NuoDirectView> WPNuoDirectView;


class NuoSwapChain;
typedef std::shared_ptr<NuoSwapChain> PNuoSwapChain;


class NuoDirectView : public NuoView
{

	PNuoSwapChain _swapChain;
	PNuoCommandQueue _commandQueue;

public:

	NuoDirectView(const PNuoWindow& parent);

	PNuoCommandQueue CommandQueue() const;

	void CreateSwapChain(const PNuoDevice& device,
						 unsigned int frameCount,
						 unsigned int w, unsigned int h);

	PNuoResource RenderTarget(unsigned int inFlight);
	D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView(unsigned int inFlight);

	void Present();
	unsigned int CurrentBackBufferIndex();

	//TODO:
	/*PNuoSwapChain SwapChain() {
		return _swapChain;
	}*/

};


