// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"

#include "ModelView.h"
#include "ModelState/ModelState.h"

#include "NuoAppInstance.h"
#include "NuoFile.h"
#include "NuoStrings.h"

#include "NuoDirect/NuoResourceSwapChain.h"
#include "NuoMeshes/NuoMeshSceneRoot.h"
#include "NuoMeshes/NuoAuxilliaryMeshes/NuoScreenSpaceMesh.h"

#include "NuoModelLoader/NuoModelLoaderGPU.h"


#include <dxcapi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>



struct InputParamType
{
    DirectX::XMFLOAT4 color;
};


ModelView::ModelView(const PNuoDevice& device,
					 const PNuoWindow& parent)
	: NuoDirectView(device, parent),
      _init(false)
{
}



PModelState ModelView::State()
{
    return _modelState;
}


void ModelView::OnSize(unsigned int x, unsigned int y)
{
	NuoDirectView::OnSize(x, y);

    const PNuoRenderTarget& renderTarget = RenderTarget(0);
    const PNuoDevice& device = CommandQueue()->Device();
    auto format = renderTarget->Format();
    auto sampleCount = renderTarget->SampleCount();
    auto w = renderTarget->Width();
    auto h = renderTarget->Height();

    _intermediateTarget = std::make_shared<NuoRenderTarget>(device, format, w, h, sampleCount, true, true);

    if (!_init)
    {
        Init();
        _init = true;
    }
}


void ModelView::Init()
{
    auto format = RenderTarget(0)->Format();
    auto sampleCount = RenderTarget(0)->SampleCount();

    _modelState = std::make_shared<ModelState>(CommandQueue(), format, sampleCount);

    PNuoCommandBuffer commandBuffer = CommandQueue()->CreateCommandBuffer();

    std::vector<PNuoResource> intermediate;
    _textureMesh = std::make_shared<NuoTextureMesh>(commandBuffer, 1);
    _textureMesh->Init(commandBuffer, intermediate, format, sampleCount);

    PNuoDevice device = CommandQueue()->Device();
    _light = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoLightUniforms));

    commandBuffer->Commit();

    // Create synchronization objects and wait until assets have been uploaded to the GPU.
    PNuoFenceSwapChain fence = device->CreateFenceSwapChain(1);
    fence->WaitForGPU(CommandQueue());
}



void ModelView::OpenFile(const std::string& path, NuoTaskProgress progress, NuoTaskCompletion completion)
{
    LoadMesh(path, progress, completion);
    Update();
}



void ModelView::Render(const PNuoCommandBuffer& commandBuffer)
{
    if (!_init)
        return;

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

    _modelState->SceneRoot()->UpdateUniform(encoder->InFlight(), NuoMatrixFloat44Identity);

    NuoMesh::CommonFunc commFunc = [&mvp, &lightBuffer](NuoCommandEncoder* encoder)
    {
        encoder->SetRootConstant(0, sizeof(NuoUniforms), &mvp);
        encoder->SetRootConstantBuffer(1, lightBuffer);
    };
    
    _modelState->SceneRoot()->DrawBegin(encoder, commFunc);
    _modelState->SceneRoot()->Draw(encoder);

	target->ReleaseRenderPassEncoder();
    encoder.reset();

    target = CurrentRenderTarget();
    encoder = target->RetainRenderPassEncoder(commandBuffer);

    encoder->SetViewport(NuoViewport());
    _textureMesh->SetTexture(nullptr, _intermediateTarget->ResultTexture());
    _textureMesh->DrawBegin(encoder, [](NuoCommandEncoder* encoder) {});
    _textureMesh->Draw(encoder);

    target->ReleaseRenderPassEncoder();
}


void ModelView::LoadMesh(const std::string& path, NuoTaskProgress progress, NuoTaskCompletion completion)
{
    NuoMeshOptions options = {};
    options._combineByMaterials = false;
    options._textured = false;
    options._basicMaterialized = true;

    _modelState->SetOptions(options);

    ModelState* modelState = _modelState.get();
    PNuoCommandQueue commandQueue = CommandQueue();

    ModelView* view = this;

    NuoTask task = [modelState, path, commandQueue, view](NuoTaskProgress progress)
    {
        modelState->LoadMesh(path, progress);

        // Create synchronization objects and wait until assets have been uploaded to the GPU.
        PNuoDevice device = commandQueue->Device();
        PNuoFenceSwapChain fence = device->CreateFenceSwapChain(1);
        fence->WaitForGPU(commandQueue);
    };

    NuoBackgroundTask::BackgroundTask(task, progress, [view, completion]()
        {
            view->Update();
            completion();
        });

    std::string documentName = LastPathComponent(path);
    _modelState->SetDocumentName(documentName);
}


void ModelView::OnMouseDown(short x, short y)
{
    EnableMouseDrag();
    NuoDirectView::OnMouseDown(x, y);
}


void ModelView::OnMouseDrag(short x, short y, short deltaX, short deltaY)
{
    float dx = deltaY * 0.002f * 3.14f;
    float dy = deltaX * 0.002f * 3.14f;

    _modelTransfer = NuoMatrixRotationAppend(_modelTransfer, dx, dy);

    Update();
}
