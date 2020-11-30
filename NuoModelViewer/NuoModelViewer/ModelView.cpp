// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "ModelView.h"

#include "NuoAppInstance.h"
#include "NuoFile.h"
#include "NuoStrings.h"

#include "NuoDirect/NuoResourceSwapChain.h"
#include "NuoMeshes/NuoMeshCompound.h"
#include "NuoMeshes/NuoAuxilliaryMeshes/NuoScreenSpaceMesh.h"

#include "NuoModelLoader/NuoModelLoader.h"
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


void ModelView::OnSize(unsigned int x, unsigned int y)
{
	NuoDirectView::OnSize(x, y);

    const PNuoRenderTarget& renderTarget = RenderTarget(0);
    const PNuoDevice& device = CommandQueue()->Device();
    auto format = renderTarget->Format();
    auto sampleCount = renderTarget->SampleCount();
    auto w = renderTarget->RenderBuffer()->Width();
    auto h = renderTarget->RenderBuffer()->Height();

    _intermediateTarget = std::make_shared<NuoRenderTarget>(device, format, w, h, sampleCount, true, true);

    if (!_init)
    {
        Init();
        _init = true;
    }
}


void ModelView::Init()
{
    PNuoDevice device = CommandQueue()->Device();

    PNuoCommandBuffer commandBuffer = CommandQueue()->CreateCommandBuffer();

    std::string path = NuoAppInstance::GetInstance()->ModulePath();
    path = RemoveLastPathComponent(path);
    path = path + "/uh60.obj";

    PNuoModelLoader loader = std::make_shared<NuoModelLoader>();
    loader->LoadModel(path);

    auto format = RenderTarget(0)->Format();
    auto sampleCount = RenderTarget(0)->SampleCount();

    NuoMeshOptions options;
    options._combineByMaterials = true;
    options._basicMaterialized = false;

    NuoModelLoaderGPU loaderGPU(loader, format, sampleCount);
    _mesh = loaderGPU.CreateMesh(options, commandBuffer, [](float) {});

    std::vector<PNuoResource> intermediate;
    _textureMesh = std::make_shared<NuoTextureMesh>(commandBuffer, BuffersCount());
    _textureMesh->Init(commandBuffer, intermediate, format, sampleCount);

    _light = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoLightUniforms));

    commandBuffer->Commit();

    // Create synchronization objects and wait until assets have been uploaded to the GPU.
    PNuoFenceSwapChain fence = device->CreateFenceSwapChain(1);
    fence->WaitForGPU(CommandQueue());
}

struct Light
{
    DirectX::XMFLOAT4 direction;
    DirectX::XMFLOAT4 ambientColor;
    DirectX::XMFLOAT4 diffuseColor;
    DirectX::XMFLOAT4 specularColor;
};


void ModelView::Render(const PNuoCommandBuffer& commandBuffer)
{
    if (!_init)
        return;

    PNuoRenderTarget target = _intermediateTarget; // CurrentRenderTarget();
    PNuoCommandEncoder encoder = target->RetainRenderPassEncoder(commandBuffer);

    encoder->SetClearColor(NuoVectorFloat4(1.0f, 1.0f, 1.0f, 1.0f));
    encoder->SetViewport(NuoViewport());

	const NuoVectorFloat3 eyePosition(0, 0, 30);
    const NuoVectorFloat3 focusPoint(0, 0, 0);
    const NuoVectorFloat3 upDirection(0, 1, 0);

    auto viewMatrix = NuoMatrixLookAt(eyePosition, focusPoint, upDirection);

    const auto w = target->RenderBuffer()->Width();
    const float h = (float)target->RenderBuffer()->Height();
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

    NuoMesh::CommonFunc commFunc = [&mvp, &lightBuffer](NuoCommandEncoder* encoder)
    {
        encoder->SetRootConstant(0, sizeof(NuoUniforms), &mvp);
        encoder->SetRootConstantBuffer(1, lightBuffer);
    };
    
    _mesh->DrawBegin(encoder, commFunc);
    _mesh->Draw(encoder);

	target->ReleaseRenderPassEncoder();
    encoder.reset();

    target = CurrentRenderTarget();
    encoder = target->RetainRenderPassEncoder(commandBuffer);

    encoder->SetViewport(NuoViewport());
    _textureMesh->SetTexture(encoder.get(), _intermediateTarget->ResultTexture());
    _textureMesh->DrawBegin(encoder, [](NuoCommandEncoder* encoder) {});
    _textureMesh->Draw(encoder);

    target->ReleaseRenderPassEncoder();
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
