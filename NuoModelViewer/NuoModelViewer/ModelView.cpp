// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"

#include "ModelView.h"

#include "ModelState/ModelState.h"
#include "UserInterface/ModelOperationPanel.h"
#include "Renderer/ModelViewerRenderer.h"
#include "Renderer/NotationRenderer.h"

#include "NuoAppInstance.h"
#include "NuoFile.h"
#include "NuoStrings.h"

#include "NuoDirect/NuoDevice.h"
#include "NuoDirect/NuoResourceSwapChain.h"
#include "NuoDirect/NuoRenderPassTarget.h"

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
      _init(false),
      _trackingLighting(false)
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
    
    _notationRenderer->SetDPI(DPI());
    
    UpdateRenderPassesDrawable();
}


void ModelView::Init()
{
    const PNuoDevice& device = CommandQueue()->Device();
    const auto format = Format();

    PNuoCommandBuffer commandBuffer = CommandQueue()->CreateCommandBuffer();

    std::vector<PNuoResource> intermediate;
    _modelRenderer = std::make_shared<ModelRenderer>(commandBuffer, BuffersCount(), intermediate, format);

    PNuoRenderTarget modelRenderTarget = std::make_shared<NuoRenderTarget>(device, format, 1, true, true);
    _modelRenderer->SetRenderTarget(modelRenderTarget);
    modelRenderTarget->SetClearColor(NuoVectorFloat4(1.0, 1.0, 1.0, 1.0));

    _notationRenderer = std::make_shared<NotationRenderer>(commandBuffer, BuffersCount(), intermediate, format);
    _notationRenderer->SetNotationWidthCap(250);

    /**
     *  match the sample of the last renderer's end-pipeline
     */
    SetSampleCount(8);

    commandBuffer->Commit();
    commandBuffer->WaitUntilComplete(intermediate);

    SetupPipelineSettings();
    ModelOptionUpdated();
}


void ModelView::SetModelPanel(const PModelOperationPanel& panel)
{
    _modelPanel = panel;
}


void ModelView::SetupPipelineSettings()
{
    SetRenderPasses({ _modelRenderer, _notationRenderer });
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
    modelState->SetOptions(options);

    ModelView* view = this;

    NuoTask task = [modelState, path](NuoTaskProgress progress)
    {
        modelState->LoadMesh(path, progress);
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
    auto lightSettingArea = _notationRenderer->NotationArea();
    NuoPoint<short> location(x, y);

    _trackingLighting = NuoRectContainsPoint(lightSettingArea, location);

    if (_trackingLighting)
    {
        _notationRenderer->SelectCurrentLightVector(location);
    }

    EnableMouseDrag();
    NuoDirectView::OnMouseDown(x, y);
}


void ModelView::OnMouseDrag(short x, short y, short deltaX, short deltaY, const NuoMouseModifer& modifier)
{
    float dx = deltaY * 0.002f * 3.14f;
    float dy = deltaX * 0.002f * 3.14f;

    if (_trackingLighting)
    {
        _notationRenderer->UpdateRotation(dx, dy);
    }
    else
    {
        _modelRenderer->SetRotationDelta(dx, dy);
    }

    Update();
}


void ModelView::OnMouseUp(short x, short y)
{
    NuoDirectView::OnMouseUp(x, y);

    _trackingLighting = false;
}


void ModelView::ModelOptionUpdated()
{
    PModelOperationPanel modelPanel = _modelPanel.lock();

    _modelRenderer->SetFieldOfView(modelPanel->FieldOfViewRadian());
    
    if (_init)
        Update();
}


