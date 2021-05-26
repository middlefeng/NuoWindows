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
#include "NuoDirect/NuoCommandBuffer.h"
#include "NuoDirect/NuoResource.h"
#include "NuoDirect/NuoRenderTarget.h"



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

	NuoDirectView(const PNuoDevice& device, const PNuoWindow& parent);

	PNuoCommandQueue CommandQueue() const;

	void CreateSwapChain(unsigned int frameCount,
						 unsigned int w, unsigned int h);

	PNuoRenderTarget RenderTarget(unsigned int inFlight);
	PNuoRenderTarget CurrentRenderTarget();
	PNuoCommandBuffer CreateCommandBuffer(bool resetAllocator);

	void Present();
	void PresentWithoutFence();
	void WaitForGPU();
	unsigned int CurrentBackBufferIndex();
	unsigned int BuffersCount();

	virtual void Render(const PNuoCommandBuffer& commandBuffer);

	virtual void OnPaint() override;
	virtual void OnSize(unsigned int x, unsigned int y) override;
	virtual void OnDestroy() override;

};


