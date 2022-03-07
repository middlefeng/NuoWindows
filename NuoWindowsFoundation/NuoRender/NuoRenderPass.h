//
//  NuoRendererPass.h
//  ModelViewer
//
//  Created by Dong Feng on 3/29/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#include <memory>
#include "NuoDirect/NuoSize.h"


class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;

class NuoRenderTarget;
typedef std::shared_ptr<NuoRenderTarget> PNuoRenderTarget;
typedef std::weak_ptr<NuoRenderTarget> WPNuoRenderTarget;

class NuoCommandEncoder;
typedef std::shared_ptr<NuoCommandEncoder> PNuoCommandEncoder;


class NuoRenderPass
{

protected:

	PNuoRenderTarget _renderTarget;

	/**
	 *  The transient target which is managed from outside
	 */
	WPNuoRenderTarget _renderTargetTransient;

public:

	virtual void SetDrawableSize(const NuoSize& size);
	virtual void SetSampleCount(unsigned int sampleCount) = 0;

	/**
	 *  draw calls that target to their own target (e.g. shadow map texture)
	 */
	virtual void PredrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) = 0;

	/**
	 *  draw calls that target to the *_renderTarget*
	 */
	virtual void DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) = 0;

	virtual bool IsPipelinePass() const;

	virtual PNuoCommandEncoder RetainDefaultEncoder(const PNuoCommandBuffer& commandBuffer);
	virtual void ReleaseDefaultEncoder();

	void SetRenderTarget(const PNuoRenderTarget& renderTarget);
	void SetTransientRenderTarget(const WPNuoRenderTarget& renderTarget);
	PNuoRenderTarget RenderTarget();

};


