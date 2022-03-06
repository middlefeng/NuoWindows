//
//  NuoRenderPipelinePass.cpp
//  ModelViewer
//
//  Created by Dong Feng on 3/30/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#include "NuoRenderPipelinePass.h"
#include "NuoMeshes/NuoAuxilliaryMeshes/NuoScreenSpaceMesh.h"



NuoRenderPipelinePass::NuoRenderPipelinePass(const PNuoCommandBuffer& commandBuffer,
                                             unsigned int frameCount,
                                             std::vector<PNuoResource>& intermediate,
                                             DXGI_FORMAT format)
{
    _textureMesh = std::make_shared<NuoTextureMesh>(commandBuffer, 1);
    _textureMesh->Init(commandBuffer, frameCount, intermediate, format);
}


void NuoRenderPipelinePass::Init(const PNuoCommandBuffer& commandBuffer, unsigned int sampleCount)
{
    _textureMesh->SetSampleCount(sampleCount);
    _textureMesh->MakePipelineState(commandBuffer);
}


void NuoRenderPipelinePass::SetSourceTextrue(const PNuoTexture& texture)
{
    _textureMesh->SetTexture(nullptr, texture);
}


void NuoRenderPipelinePass::DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer)
{
    PNuoCommandEncoder encoder = RetainDefaultEncoder(commandBuffer);

    encoder->SetViewport(NuoViewport());

    _textureMesh->DrawBegin(encoder, [](NuoCommandEncoder* encoder) {});
    _textureMesh->Draw(encoder);

    ReleaseDefaultEncoder();
}


bool NuoRenderPipelinePass::IsPipelinePass() const
{
    return true;
}
