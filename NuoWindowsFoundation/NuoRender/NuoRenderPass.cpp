//
//  NuoRendererPass.cpp
//  ModelViewer
//
//  Created by Dong Feng on 3/29/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#include "NuoRenderPass.h"

#include "NuoDirect/NuoRenderTarget.h"



void NuoRenderPass::SetDrawableSize(const NuoSize& size)
{
	if (_renderTarget)
		_renderTarget->SetDrawableSize(size);
}



bool NuoRenderPass::IsPipelinePass() const
{
	return false;
}


PNuoCommandEncoder NuoRenderPass::RetainDefaultEncoder(const PNuoCommandBuffer& commandBuffer)
{
	return RenderTarget()->RetainRenderPassEncoder(commandBuffer);
}


void NuoRenderPass::ReleaseDefaultEncoder()
{
	RenderTarget()->ReleaseRenderPassEncoder();
}


void NuoRenderPass::SetTransientRenderTarget(const WPNuoRenderTarget& renderTarget)
{
	_renderTargetTransient = renderTarget;
}


PNuoRenderTarget NuoRenderPass::RenderTarget()
{
	if (_renderTarget)
		return _renderTarget;

	auto transientTarget = _renderTargetTransient.lock();
	return transientTarget;
}


