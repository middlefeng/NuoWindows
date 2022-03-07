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
    
    UpdateRenderPassesDrawable();
}


void ModelView::Init()
{
    const PNuoRenderTarget& renderTarget = RenderTarget(0);
    const PNuoDevice& device = CommandQueue()->Device();

    auto format = renderTarget->Format();

    PNuoCommandBuffer commandBuffer = CommandQueue()->CreateCommandBuffer();

    std::vector<PNuoResource> intermediate;
    _modelRenderer = std::make_shared<ModelRenderer>(commandBuffer, BuffersCount(), intermediate, format);

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

    _modelRenderer->Rotate(dx, dy);

    Update();
}
