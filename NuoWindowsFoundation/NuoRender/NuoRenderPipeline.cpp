//
//  NuoRenderPipeline.cpp
//  ModelViewer
//
//  Created by Dong Feng on 5/23/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#include "NuoRenderPipeline.h"
#include "NuoRenderPipelinePass.h"

#include "NuoDirect/NuoRenderPassTarget.h"



void NuoRenderPipeline::RenderWithCommandBuffer(const PNuoCommandBuffer& commandBuffer)
{
    // rendering that do not need the drawable (which is subject to the limit of
    // the render surface frame buffers, therefore might cause wait)
    //
    for (PNuoRenderPass pass : _renderPasses)
    {
        pass->PredrawWithCommandBuffer(commandBuffer);
    }
    
    // associate the source and destine texture of each step, along the course
    // of rendering each step
    //
    const size_t count = _renderPasses.size();
    for (size_t i = 0; i < count; ++i)
    {
        const PNuoRenderPass renderStep = _renderPasses[i];
        if (!renderStep->IsPipelinePass())
            continue;
        
        PNuoRenderPipelinePass renderStepSuccessor;
        
        if (i < count - 1)
            renderStepSuccessor = std::dynamic_pointer_cast<NuoRenderPipelinePass>(_renderPasses[i + 1]);
        
        if (renderStepSuccessor)
        {
            PNuoRenderTarget interResult = renderStep->RenderTarget();
            renderStepSuccessor->SetSourceTextrue(interResult->ResultTexture());
        }
        
        renderStep->DrawWithCommandBuffer(commandBuffer);
    }
}


void NuoRenderPipeline::SetRenderPasses(const std::vector<PNuoRenderPass>& passes)
{
    _renderPasses = passes;
}


void NuoRenderPipeline::SetDrawableSize(const NuoSize& size)
{
    for (size_t i = 0; i < _renderPasses.size(); ++i)
    {
        PNuoRenderPass render = _renderPasses[i];
        render->SetDrawableSize(size);
    }
}

/*
- (void)setSampleCount:(NSUInteger)sampleCount
{
    for (size_t i = 0; i < [_renderPasses count]; ++i)
    {
        NuoRenderPass* render = _renderPasses[i];
        [render setSampleCount:sampleCount];
    }
}



@end
*/