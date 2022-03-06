
//
//  NotationRenderer.cpp
//  ModelViewer
//
//  Created by Dong Feng on 12/23/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//


#include "NotationRenderer.h"


NotationRenderer::NotationRenderer(const PNuoCommandBuffer& commandBuffer,
								   unsigned int frameCount,
								   std::vector<PNuoResource>& intermediate,
								   DXGI_FORMAT format)
	: NuoRenderPipelinePass(commandBuffer, frameCount, intermediate, format)
{
	Init(commandBuffer, 1);
}



void NotationRenderer::SetDrawableSize(const NuoSize& size)
{
}


void NotationRenderer::SetSampleCount(unsigned int sampleCount)
{
}


void NotationRenderer::PredrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer)
{
}


void NotationRenderer::DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer)
{
	NuoRenderPipelinePass::DrawWithCommandBuffer(commandBuffer);
}

