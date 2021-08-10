// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"

#include "ModelView.h"

#include "ModelState/ModelState.h"
#include "Renderer/ModelViewerRenderer.h"

#include "NuoAppInstance.h"
#include "NuoFile.h"
#include "NuoStrings.h"

#include "NuoDirect/NuoDevice.h"
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
    return _modelRenderer->State();
}


void ModelView::OnSize(unsigned int x, unsigned int y)
{
	NuoDirectView::OnSize(x, y);

    if (!_init)
    {
        Init();
        _init = true;
    }
    
    const PNuoRenderTarget& renderTarget = RenderTarget(0);
    const auto size = renderTarget->DrawableSize();

    //_intermediateTarget->SetDrawableSize(size);
    _modelRenderer->SetDrawableSize(size);
}


void ModelView::Init()
{
    const PNuoRenderTarget& renderTarget = RenderTarget(0);
    const PNuoDevice& device = CommandQueue()->Device();

    auto format = renderTarget->Format();
    auto w = renderTarget->Width();
    auto h = renderTarget->Height();

    /*
    // the intermediate target takes the direct render of the model so its sample count is the
    // MSAA sample count
    //
    auto modelSampleCount = 8;
    _intermediateTarget = std::make_shared<NuoRenderTarget>(device, format, w, h, modelSampleCount, true, true);
    _modelState = std::make_shared<ModelState>(CommandQueue(), format);*/

    PNuoCommandBuffer commandBuffer = CommandQueue()->CreateCommandBuffer();

    /*auto sampleCount = renderTarget->SampleCount();*/
    std::vector<PNuoResource> intermediate;
    /*_textureMesh = std::make_shared<NuoTextureMesh>(commandBuffer, 1);
    _textureMesh->Init(commandBuffer, BuffersCount(), intermediate, format);
    _textureMesh->SetSampleCount(1);
    _textureMesh->MakePipelineState(commandBuffer);

    _light = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoLightUniforms));
    _mvp = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoUniforms)); */

    _modelRenderer = std::make_shared<ModelRenderer>(commandBuffer, BuffersCount(), intermediate, format, w, h);

    commandBuffer->Commit();

    // Create synchronization objects and wait until assets have been uploaded to the GPU.
    PNuoFenceSwapChain fence = device->CreateFenceSwapChain(1);
    fence->WaitForGPU(CommandQueue());

    SetRenderPasses({ _modelRenderer });
}


void ModelView::SetupPipelineSettings()
{
    
}



void ModelView::OpenFile(const std::string& path, NuoTaskProgress progress)
{
    LoadMesh(path, progress);
    Update();
}



void ModelView::Render(const PNuoCommandBuffer& commandBuffer)
{
    if (!_init)
        return;

    NuoDirectView::Render(commandBuffer);

    /*if (!_modelState->SceneRoot())
        return;

    /*PNuoRenderTarget target = _intermediateTarget; // CurrentRenderTarget();
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

    target = CurrentRenderTarget();
    encoder = target->RetainRenderPassEncoder(commandBuffer);

    encoder->SetViewport(NuoViewport());
    _textureMesh->SetTexture(nullptr, _intermediateTarget->ResultTexture());
    _textureMesh->DrawBegin(encoder, [](NuoCommandEncoder* encoder) {});
    _textureMesh->Draw(encoder);

    target->ReleaseRenderPassEncoder();*/
}


void ModelView::LoadMesh(const std::string& path, NuoTaskProgress progress)
{
    NuoMeshOptions options = {};
    options._combineByMaterials = false;
    options._textured = false;
    options._basicMaterialized = true;

    options._frameCount = BuffersCount();
    options._sampleCount = 8;  // MSAA sample count

    PModelState modelState = _modelRenderer->State();
    PNuoCommandQueue commandQueue = CommandQueue();

    modelState->SetOptions(options);

    ModelView* view = this;

    NuoTask task = [modelState, path, commandQueue, view](NuoTaskProgress progress)
    {
        modelState->LoadMesh(path, progress);

        // Create synchronization objects and wait until assets have been uploaded to the GPU.
        PNuoDevice device = commandQueue->Device();
        PNuoFenceSwapChain fence = device->CreateFenceSwapChain(1);
        fence->WaitForGPU(commandQueue);
    };

    NuoBackgroundTask backgroundTask(task);

    std::string documentName = LastPathComponent(path);
    modelState->SetDocumentName(documentName);

    float progressRate = 0;
    while (backgroundTask.Resume(&progressRate))
    {
        progress(progressRate);
    }

    view->Update();
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

    //_modelTransfer = NuoMatrixRotationAppend(_modelTransfer, dx, dy);
    _modelRenderer->Rotate(dx, dy);

    Update();
}
