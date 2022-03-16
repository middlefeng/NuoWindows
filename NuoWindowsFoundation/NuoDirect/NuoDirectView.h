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

class NuoRenderPipeline;
typedef std::shared_ptr<NuoRenderPipeline> PNuoRenderPipeline;

class NuoRenderPass;
typedef std::shared_ptr<NuoRenderPass> PNuoRenderPass;


class NuoDirectView : public NuoView
{

	PNuoSwapChain _swapChain;
	PNuoCommandQueue _commandQueue;

	PNuoRenderPipeline _renderPipeline;

public:

	NuoDirectView(const PNuoDevice& device, const PNuoWindow& parent);

	PNuoCommandQueue CommandQueue() const;

	void CreateSwapChain(unsigned int frameCount,
						 unsigned int w, unsigned int h);

	/**
	 *   the code used to get the format by query it from a target (usually the 0th target in the
	 *   swap-chain). that method is problematic as it touch the swap-chain resources unncessary.
	 *   when a swap-chain is associated with some resources, it has to be presented and then 
	 *   destroy all those resources associated with the previous frame, before some operation
	 *   (noticeably the buffer-resize) could be performed.
	 */
	DXGI_FORMAT Format();

	PNuoRenderTarget RenderTarget(unsigned int inFlight);
	PNuoRenderTarget CurrentRenderTarget();
	PNuoCommandBuffer CreateCommandBuffer(bool resetAllocator);

	void Present();
	void PresentWithoutFence();
	void WaitForGPU();
	unsigned int CurrentBackBufferIndex();
	unsigned int BuffersCount();

	void SetRenderPasses(const std::vector<PNuoRenderPass>& passes);
	void UpdateRenderPassesDrawable();

	virtual void Render(const PNuoCommandBuffer& commandBuffer);

	virtual void OnPaint() override;
	virtual void OnSize(unsigned int x, unsigned int y) override;
	virtual void OnDestroy() override;

};


