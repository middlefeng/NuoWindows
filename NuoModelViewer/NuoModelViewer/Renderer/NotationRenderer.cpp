
//
//  NotationRenderer.cpp
//  ModelViewer
//
//  Created by Dong Feng on 12/23/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//


#include "NotationRenderer.h"

#include "NuoDirect/NuoRenderTarget.h"
#include "NuoDirect/NuoResourceSwapChain.h"


NotationRenderer::NotationRenderer(const PNuoCommandBuffer& commandBuffer,
								   unsigned int frameCount,
								   std::vector<PNuoResource>& intermediate,
								   DXGI_FORMAT format)
	: NuoRenderPipelinePass(commandBuffer, frameCount, intermediate, format)
{
	Init(commandBuffer, 8);

	_lightNotation = std::make_shared<NotationLight>(commandBuffer, frameCount, intermediate, format, true);

    const PNuoCommandQueue& commandQueue = commandBuffer->CommandQueue();
    const PNuoDevice& device = commandQueue->Device();
    _lightBuffer = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoLightUniforms));
    _transforms = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoUniforms));
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
	PNuoRenderTarget target = RenderTarget();
	PNuoCommandEncoder encoder = target->RetainRenderPassEncoder(commandBuffer);

	NuoRenderPipelinePass::DrawWithCommandBuffer(commandBuffer);

	const float w = (float)target->Width();
	const float h = (float)target->Height();

	NuoViewport viewport(w/2, h/2, w / 2, h / 2, 0, 1.0);
    encoder->SetViewport(viewport);

    const PNuoResourceSwapChain& lightBuffer = _lightBuffer;
    const PNuoResourceSwapChain& mvpBuffer = _transforms;

    NuoMesh::CommonFunc commFunc = [&mvpBuffer, &lightBuffer](NuoCommandEncoder* encoder)
    {
        encoder->SetRootConstantBuffer(0, mvpBuffer);
        encoder->SetRootConstantBuffer(1, lightBuffer);
    };

    UpdateUniformsForView(encoder);

	_lightNotation->DrawWithRenderPass(encoder, commFunc);

    target->ReleaseRenderPassEncoder();
}


void NotationRenderer::UpdateUniformsForView(const PNuoRenderInFlight& inFlight)
{
    PNuoRenderTarget target = RenderTarget();

    const NuoVectorFloat3 eyePosition(0, 0, 30);
    const NuoVectorFloat3 focusPoint(0, 0, 0);
    const NuoVectorFloat3 upDirection(0, 1, 0);

    const auto viewMatrix = NuoMatrixLookAt(eyePosition, focusPoint, upDirection);
    const auto viewMatrixInverse = viewMatrix.Inverse();

    const auto w = target->Width();
    const float h = (float)target->Height();
    const float aspectRatio = w / h;
    NuoMatrixFloat44 projectionMatrix = NuoMatrixPerspective(aspectRatio, DirectX::XMConvertToRadians(20), 0.1f, 100.f);

    NuoMatrixFloat44 normalMatrix = NuoMatrixExtractLinear(viewMatrix);
    NuoMatrixFloat44 vpMatrix = projectionMatrix * viewMatrix;

    NuoUniforms mvp;
    mvp.viewProjectionMatrix = vpMatrix._m;
    mvp.viewMatrix = viewMatrix._m;
    mvp.viewMatrixInverse = viewMatrixInverse._m;

    const PNuoResourceSwapChain& lightBuffer = _lightBuffer;
    NuoLightUniforms light;
    light.lightParams[0].direction = NuoVectorFloat4(0.13f, 0.72f, 0.68f, 0.f)._vector;
    light.lightParams[0].irradiance = 1.0;
    lightBuffer->UpdateResource(&light, sizeof(NuoLightUniforms), inFlight->InFlight());

    const PNuoResourceSwapChain& mvpBuffer = _transforms;
    mvpBuffer->UpdateResource(&mvp, sizeof(NuoUniforms), inFlight->InFlight());
}
