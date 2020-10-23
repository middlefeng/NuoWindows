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



class NuoSwapChain : public std::enable_shared_from_this<NuoSwapChain>
{
	Microsoft::WRL::ComPtr<IDXGISwapChain3> _swapChain;
	WPNuoDevice _device;
	WPNuoDirectView _view;

	PNuoResourceSwapChain _buffer;
	PNuoRenderTargetSwapChain _rtvSwapChain;

public:

	NuoSwapChain(const PNuoDirectView& view,
				 unsigned int frameCount,
				 unsigned int w, unsigned int h);

	PNuoResourceSwapChain Buffer();
	PNuoRenderTargetSwapChain RenderTargetViews();

	// TODO:
	unsigned int CurrentBackBufferIndex();
	IDXGISwapChain3* DxChain() const;
};


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

	//TODO:
	PNuoSwapChain SwapChain() {
		return _swapChain;
	}

};


