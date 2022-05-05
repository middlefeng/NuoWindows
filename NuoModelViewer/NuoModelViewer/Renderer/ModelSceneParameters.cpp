//
//  ModelSceneParameters.m
//  ModelViewer
//
//  Created by Dong on 7/26/19.
//  Ported 5/4/22
//  Copyright © 2022 Dong Feng. All rights reserved.
//



#include "ModelSceneParameters.h"

#include "NuoMeshes/NuoShaders/NuoUniforms.h"


ModelSceneParameters::ModelSceneParameters(const PNuoDevice& device)
{
	_transUniformBuffers = std::make_shared<NuoResourceSwapChain>(device, 3, (unsigned long)sizeof(NuoUniforms));
}



void ModelSceneParameters::UpdateUniforms(const PNuoCommandBuffer& commandBuffer)
{
    const NuoMatrixFloat44& viewTrans = _viewMatrix;

    const CGSize& drawableSize = _drawableSize;
    const float aspect = drawableSize.width / drawableSize.height;

    // bounding box transform and determining the near/far
    //
    NuoBounds bounds = [_sceneRoot worldBounds : viewTrans].boundingBox;

    float near = -bounds._center.z() - bounds._span.z() / 2.0 + 0.01;
    float far = near + bounds._span.z() + 0.02;
    near = std::max<float>(0.001, near);
    far = std::max<float>(near + 0.001, far);

    _projection = NuoMatrixPerspective(aspect, _fieldOfView, near, far);

    NuoUniforms uniforms;
    uniforms.viewMatrix = viewTrans._m;
    uniforms.viewMatrixInverse = viewTrans.Inverse()._m;
    uniforms.viewProjectionMatrix = (_projection * viewTrans)._m;

    [_transUniformBuffers updateBufferWithInFlight : commandBuffer withContent : &uniforms] ;

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

