


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



ModelRenderer::ModelRenderer(const PNuoCommandBuffer& commandBuffer, unsigned int frameCount,
                             std::vector<PNuoResource>& intermediate, DXGI_FORMAT format)
    : NuoRenderPipelinePass(commandBuffer, frameCount, intermediate, format)
{
    const PNuoCommandQueue& commandQueue = commandBuffer->CommandQueue();
    const PNuoDevice& device = commandQueue->Device();

    // the intermediate target takes the direct render of the model so its sample count is the
    // MSAA sample count
    //
    auto modelSampleCount = 8;
    _intermediateTarget = std::make_shared<NuoRenderTarget>(device, format, modelSampleCount, true, true);
    _modelState = std::make_shared<ModelState>(commandQueue, format);

    _textureMesh = std::make_shared<NuoTextureMesh>(commandBuffer, 1);
    _textureMesh->Init(commandBuffer, frameCount, intermediate, format);
    _textureMesh->SetSampleCount(1);
    _textureMesh->MakePipelineState(commandBuffer);

    _light = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoLightUniforms));
    _mvp = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoUniforms));

    _lightNotation = std::make_shared<NotationLight>(commandBuffer, frameCount, intermediate, format, true);
}



PModelState ModelRenderer::State() const
{
    return _modelState;
}



void ModelRenderer::SetDrawableSize(const NuoSize& size)
{
    NuoRenderPipelinePass::SetDrawableSize(size);
    _intermediateTarget->SetDrawableSize(size);
}



void ModelRenderer::SetSampleCount(unsigned int sampleCount)
{
    // TODO:
}



void ModelRenderer::PredrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer)
{
}



void ModelRenderer::DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer)
{
    if (!_modelState->SceneRoot())
        return;

    PNuoRenderTarget target = _intermediateTarget;
    PNuoCommandEncoder encoder = target->RetainRenderPassEncoder(commandBuffer);

    {
        const auto w = target->Width();
        const float h = (float)target->Height();

        NuoViewport viewport(0, 0, w, h, 0, 1.0);

        encoder->SetClearColor(NuoVectorFloat4(1.0f, 1.0f, 1.0f, 1.0f));
        encoder->SetViewport(viewport);

    }

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

    {
        const auto w = target->Width();
        const float h = (float)target->Height();

        NuoViewport viewport(0, 0, w/2, h/2, 0, 1.0);

        encoder->SetViewport(viewport);

    }

    _lightNotation->DrawWithRenderPass(encoder, commFunc);

    target->ReleaseRenderPassEncoder();
    encoder.reset();

    target = RenderTarget();

    encoder = target->RetainRenderPassEncoder(commandBuffer);

    encoder->SetViewport(NuoViewport());
    _textureMesh->SetTexture(nullptr, _intermediateTarget->ResultTexture());
    _textureMesh->DrawBegin(encoder, [](NuoCommandEncoder* encoder) {});
    _textureMesh->Draw(encoder);

    target->ReleaseRenderPassEncoder();
}


void ModelRenderer::Rotate(float dx, float dy)
{
    _modelState->Rotate(dx, dy);
}

