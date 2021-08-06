


#include "ModelViewerRenderer.h"

#include "ModelState/ModelState.h"

#include "NuoMeshes/NuoMeshSceneRoot.h"
#include "NuoMeshes/NuoShaders/NuoUniforms.h"
#include "NuoMeshes/NuoAuxilliaryMeshes/NuoScreenSpaceMesh.h"

#include "NuoDirect/NuoCommandQueue.h"
#include "NuoDirect/NuoCommandBuffer.h"
#include "NuoDirect/NuoRenderTarget.h"
#include "NuoDirect/NuoResourceSwapChain.h"

#include <memory>



ModelRenderer::ModelRenderer(const PNuoCommandBuffer& commandBuffer, std::vector<PNuoResource>& intermediate, DXGI_FORMAT format,
                             unsigned int width, unsigned int height, unsigned int sampleCount)
    : NuoRenderPipelinePass(commandBuffer, intermediate, format, sampleCount)
{
    const PNuoCommandQueue& commandQueue = commandBuffer->CommandQueue();
    const PNuoDevice& device = commandQueue->Device();

    // the intermediate target takes the direct render of the model so its sample count is the
    // MSAA sample count
    //
    auto modelSampleCount = 8;
    _intermediateTarget = std::make_shared<NuoRenderTarget>(device, format, width, height, modelSampleCount, true, true);
    _modelState = std::make_shared<ModelState>(commandQueue, format, modelSampleCount);

    _textureMesh = std::make_shared<NuoTextureMesh>(commandBuffer, 1);
    _textureMesh->Init(commandBuffer, intermediate, format, sampleCount);

    _light = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoLightUniforms));
    _mvp = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoUniforms));
}



PModelState ModelRenderer::State() const
{
    return _modelState;
}



void ModelRenderer::SetDrawableSize(const NuoSize& size)
{
    _intermediateTarget->SetDrawableSize(size);
}



void ModelRenderer::DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer)
{
    if (!_modelState->SceneRoot())
        return;

    PNuoRenderTarget target = _intermediateTarget; // CurrentRenderTarget();
    PNuoCommandEncoder encoder = target->RetainRenderPassEncoder(commandBuffer);

    encoder->SetClearColor(NuoVectorFloat4(1.0f, 1.0f, 1.0f, 1.0f));
    encoder->SetViewport(NuoViewport());

    const NuoVectorFloat3 eyePosition(0, 0, 30);
    const NuoVectorFloat3 focusPoint(0, 0, 0);
    const NuoVectorFloat3 upDirection(0, 1, 0);

    auto viewMatrix = NuoMatrixLookAt(eyePosition, focusPoint, upDirection);

    const auto w = target->Width();
    const float h = (float)target->Height();
    const float aspectRatio = w / h;
    NuoMatrixFloat44 projectionMatrix = NuoMatrixPerspective(aspectRatio, DirectX::XMConvertToRadians(20), 0.1f, 100.f);

    NuoMatrixFloat44 mvpMatrix = viewMatrix * _modelTransfer;
    NuoMatrixFloat44 normalMatrix = NuoMatrixExtractLinear(mvpMatrix);
    mvpMatrix = projectionMatrix * mvpMatrix;

    NuoUniforms mvp;
    mvp.viewProjectionMatrix = mvpMatrix._m;
    mvp.viewMatrix = (viewMatrix * _modelTransfer)._m;

    const PNuoResourceSwapChain& lightBuffer = _light;
    NuoLightUniforms light;
    light.lightParams[0].direction = NuoVectorFloat4(0.13f, 0.72f, 0.68f, 0.f)._vector;
    light.lightParams[0].irradiance = 1.0;
    lightBuffer->UpdateResource(&light, sizeof(NuoLightUniforms), encoder->InFlight());

    const PNuoResourceSwapChain& mvpBuffer = _mvp;
    _mvp->UpdateResource(&mvp, sizeof(NuoUniforms), encoder->InFlight());

    _modelState->SceneRoot()->UpdateUniform(encoder->InFlight(), NuoMatrixFloat44Identity);

    NuoMesh::CommonFunc commFunc = [&mvpBuffer, &lightBuffer](NuoCommandEncoder* encoder)
    {
        encoder->SetRootConstantBuffer(0, mvpBuffer);
        encoder->SetRootConstantBuffer(1, lightBuffer);
    };

    _modelState->SceneRoot()->DrawBegin(encoder, commFunc);
    _modelState->SceneRoot()->Draw(encoder);

    target->ReleaseRenderPassEncoder();
    encoder.reset();

    target = RenderTarget();
    encoder = target->RetainRenderPassEncoder(commandBuffer);

    encoder->SetViewport(NuoViewport());
    _textureMesh->SetTexture(nullptr, _intermediateTarget->ResultTexture());
    _textureMesh->DrawBegin(encoder, [](NuoCommandEncoder* encoder) {});
    _textureMesh->Draw(encoder);
}

