
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

#define _USE_MATH_DEFINES
#include <math.h>


static NuoLightUniforms* kLightUniform = 0;


NotationRenderer::NotationRenderer(const PNuoCommandBuffer& commandBuffer,
								   unsigned int frameCount,
								   std::vector<PNuoResource>& intermediate,
								   DXGI_FORMAT format)
	: NuoRenderPipelinePass(commandBuffer, frameCount, intermediate, format),
      _notationWidthCap(0)
{
	Init(commandBuffer, 8);

    for (unsigned int index = 0; index < 4; ++index)
    {
        PNotationLight lightNotation = std::make_shared<NotationLight>(commandBuffer, frameCount, intermediate, format,
                                                                       index < 2 /* the first two with shadow casting */ );
        _lightVectors.push_back(lightNotation);

        /*NuoLightSource* lightSource = [[NuoLightSource alloc]init];
        lightSource.enableShadow = (index < 2);

        lightNotation.lightSourceDesc = lightSource;
        [lightSourcesDesc addObject : lightSource] ;*/
    }

    _currentLightVector = _lightVectors[0];
    _currentLightVector.lock()->SetSelected(true);

    // the direction of light used to render the "light vector"
    //
    if (!kLightUniform)
    {
        kLightUniform = new NuoLightUniforms;
        memset(kLightUniform, 0, sizeof(NuoLightUniforms));

        kLightUniform->lightParams[0].direction = { 0.13f, 0.72f, 0.68f, 0.0f };
        kLightUniform->lightParams[0].irradiance = 1.0f;
        kLightUniform->lightParams[0].specular = 0.6f;
    }

    const PNuoCommandQueue& commandQueue = commandBuffer->CommandQueue();
    const PNuoDevice& device = commandQueue->Device();
    _lightBuffer = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoLightUniforms));
    _transforms = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoUniforms));
}


void NotationRenderer::SelectCurrentLightVector(const NuoPoint<short>& point)
{
    const NuoPoint normalized
    (
        ((point.X() - _notationArea.X()) / _notationArea.W() * 2.0 - 1.0),
        ((point.Y() - _notationArea.Y()) / _notationArea.H() * 2.0 - 1.0) * (-.1)   /* window-coordinate has Y pointing down,
                                                                                       directx NDC has Y pointing up */
    );

    double minDistance = 2.0;
    PNotationLight deselected = _currentLightVector.lock();
    PNotationLight selected;

    for (size_t i = 0; i < _lightVectors.size(); ++i)
    {
        NuoPoint<float> headProjected = _lightVectors[i]->HeadPointProjectedWithView(NuoMatrixFloat44Identity /*_modelState.viewRotationMatrix */);
        double distance = sqrt((headProjected.X() - normalized.X()) * (headProjected.X() - normalized.X()) +
                              (headProjected.Y() - normalized.Y()) * (headProjected.Y() - normalized.Y()));
        if (distance < minDistance)
        {
            minDistance = distance;
            _currentLightVector = _lightVectors[i];
            selected = _lightVectors[i];
        }
    }

    if (deselected != selected)
    {
        selected->SetSelected(true);
        deselected->SetSelected(false);
    }
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

    const float lightSettingAreaFactor = 0.28f;
    //const float lightSlidersHeight = 140;

	const float w = (float)target->Width();
	const float h = (float)target->Height();

    const float viewPortWidth = fmin(w * lightSettingAreaFactor, _notationWidthCap * _dpiFactor);
    const float viewPortHeight = fmin(h * lightSettingAreaFactor, _notationWidthCap * _dpiFactor);
    const float viewPortOriginX = w - viewPortWidth;
    const float viewPortOriginY = h - viewPortHeight;

    _notationArea.SetX(viewPortOriginX);
    _notationArea.SetY(viewPortOriginY);
    _notationArea.SetW(viewPortWidth);
    _notationArea.SetH(viewPortHeight);

	NuoViewport viewport(viewPortOriginX, viewPortOriginY,
                         viewPortWidth, viewPortHeight, 0, 1.0);
   
    encoder->SetViewport(viewport);

    const PNuoResourceSwapChain& lightBuffer = _lightBuffer;
    const PNuoResourceSwapChain& mvpBuffer = _transforms;

    NuoMesh::CommonFunc commFunc = [&mvpBuffer, &lightBuffer](NuoCommandEncoder* encoder)
    {
        encoder->SetRootConstantBuffer(0, mvpBuffer);
        encoder->SetRootConstantBuffer(1, lightBuffer);
    };

    UpdateUniformsForView(encoder);

    for (PNotationLight notationLight : _lightVectors)
    {
        notationLight->DrawWithRenderPass(encoder, commFunc);
    }

    target->ReleaseRenderPassEncoder();
}


void NotationRenderer::SetNotationWidthCap(float cap)
{
    _notationWidthCap = cap;
}


void NotationRenderer::SetDPI(float dpi)
{
    _dpiFactor = dpi;
}


NuoRect<float> NotationRenderer::NotationArea() const
{
    return _notationArea;
}


void NotationRenderer::UpdateRotation(float deltaX, float deltaY)
{
    const NuoMatrixFloat44 updateMatrix = NuoMatrixRotation(deltaX, deltaY);
    const NuoMatrixFloat44 viewRotation = NuoMatrixFloat44Identity; // TODO: _modelState.viewRotationMatrix;

    PNotationLight currentLight = _currentLightVector.lock();

    currentLight->UpdateLightTransform(viewRotation.Inverse() * updateMatrix * viewRotation);
}


void NotationRenderer::UpdateUniformsForView(const PNuoRenderInFlight& inFlight)
{
    PNuoRenderTarget target = RenderTarget();

    const NuoMeshBounds meshBounds = _lightVectors[0]->Bounds();
    const NuoBounds& bounds = meshBounds.boundingBox;
    const float modelSpan = bounds.MaxDimension();

    const float zoom = -200.0;

    const float modelNearest = -modelSpan;
    const float cameraDefaultDistance = (modelNearest - modelSpan);
    const float cameraDistance = cameraDefaultDistance + zoom * modelSpan / 20.0f;

    const NuoVectorFloat3 cameraTranslation(0, 0, cameraDistance);
    const NuoMatrixFloat44 viewMatrix = NuoMatrixTranslation(cameraTranslation); // TODO: *_modelState.viewRotationMatrix;

    const float aspect = _notationArea.W() / _notationArea.H();
    const float nearBound = -cameraDistance - modelSpan;
    const float farBound = nearBound + modelSpan * 2.0f;
    const NuoMatrixFloat44 projectionMatrix = NuoMatrixPerspective(aspect, (float)((2 * M_PI) / 30.f), nearBound, farBound);

    NuoUniforms uniforms;
    uniforms.viewMatrix = viewMatrix._m;
    uniforms.viewMatrixInverse = viewMatrix.Inverse()._m;
    uniforms.viewProjectionMatrix = (projectionMatrix * viewMatrix)._m;

    _transforms->UpdateResource(&uniforms, sizeof(NuoUniforms), inFlight);

    NuoLightUniforms lightUniforms = *kLightUniform;

    /* TODO: view rotation handling
     * 
    auto vector = lightUniforms.lightParams[0].direction;
    NuoVectorFloat4 lightVector(vector.x, vector.y, vector.z, 0.0);

    lightVector = _modelState.viewRotationMatrix.Inverse() * lightVector;
    lightUniforms.lightParams[0].direction = lightVector._vector; */
    
    _lightBuffer->UpdateResource(&lightUniforms, sizeof(NuoLightUniforms), inFlight);
}
