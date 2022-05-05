//
//  ModelSceneParameters.m
//  ModelViewer
//
//  Created by Dong on 7/26/19.
//  Ported 5/4/22
//  Copyright © 2022 Dong Feng. All rights reserved.
//



#include "ModelSceneParameters.h"

#include "NuoModelLoader/NuoBounds.h"
#include "NuoMeshes/NuoMeshSceneRoot.h"
#include "NuoMeshes/NuoShaders/NuoUniforms.h"
#include "NuoDirect/NuoResourceSwapChain.h"


ModelSceneParameters::ModelSceneParameters(const PNuoDevice& device)
    : _drawableSize(0, 0)
{
	_transUniformBuffers = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoUniforms));
}



void ModelSceneParameters::UpdateUniforms(const PNuoCommandBuffer& commandBuffer)
{
    const NuoMatrixFloat44& viewTrans = _viewMatrix;

    const NuoSize& drawableSize = _drawableSize;
    const float aspect = drawableSize.X() / ((float)drawableSize.Y());

    // bounding box transform and determining the near/far
    //
    NuoBounds bounds = _sceneRoot->WorldBounds(viewTrans).boundingBox;

    float nearPlane = -bounds._center.z() - bounds._span.z() / 2.0 + 0.01;
    float farPlane = nearPlane + bounds._span.z() + 0.02;
    nearPlane = std::max<float>(0.001, nearPlane);
    farPlane = std::max<float>(nearPlane + 0.001, farPlane);

    _projection = NuoMatrixPerspective(aspect, _fieldOfView, nearPlane, farPlane);

    NuoUniforms uniforms;
    uniforms.viewMatrix = viewTrans._m;
    uniforms.viewMatrixInverse = viewTrans.Inverse()._m;
    uniforms.viewProjectionMatrix = (_projection * viewTrans)._m;

    _transUniformBuffers->UpdateResource(&uniforms, sizeof(NuoUniforms), commandBuffer);

    /* TODO:
    NuoLightUniforms lighting;
    lighting.ambient = _ambient._vector;
    for (unsigned int i = 0; i < 4; ++i)
    {
        const NuoVectorFloat4 lightVector(_lights[i].lightDirection * NuoVectorFloat4(0, 0, 1, 0));
        lighting.lightParams[i].direction = lightVector._vector;
        lighting.lightParams[i].irradiance = _lights[i].lightingIrradiance;
        lighting.lightParams[i].specular = _lights[i].lightingSpecular;

        if (i < 2)
        {
            lighting.shadowParams[i].soften = _lights[i].shadowSoften;
            lighting.shadowParams[i].bias = _lights[i].shadowBias;
            lighting.shadowParams[i].occluderRadius = _lights[i].shadowOccluderRadius;
        }
    }

    [_lightingUniformBuffers updateBufferWithInFlight : commandBuffer withContent : &lighting]; */
}

