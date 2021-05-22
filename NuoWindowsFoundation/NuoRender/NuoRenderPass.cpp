//
//  NuoRendererPass.cpp
//  ModelViewer
//
//  Created by Dong Feng on 3/29/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#include "NuoRenderPass.h"

#include "NuoDirect/NuoRenderTarget.h"



bool NuoRenderPass::IsPipelinePass() const
{
	return false;
}


PNuoCommandEncoder NuoRenderPass::RetainDefaultEncoder(const PNuoCommandBuffer& commandBuffer)
{
	return _renderTarget->RetainRenderPassEncoder(commandBuffer);
}


void NuoRenderPass::ReleaseDefaultEncoder()
{
	_renderTarget->ReleaseRenderPassEncoder();
}
