//
//  NotationLight.cpp
//  ModelViewer
//
//  Created by middleware on 11/13/16.
//  Ported: 9/23/21
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#include "NotationLight.h"

#include "NuoMeshes/NuoMesh.h"
#include "NuoMeshes/NuoMeshBounds.h"
#include "NuoMeshes/NuoShaders/NuoUniforms.h"


#include "NuoMathVector.h"

#include "NuoModelArrow.h"
#include <memory>

#include "NuoShadowMap/NuoLightSource.h"



class NotationLightMesh : public NuoMeshSimple
{

    PNuoResource _characterUniformBuffer;

public:

    NotationLightMesh(const PNuoCommandBuffer& commandBuffer);

    virtual PNuoRootSignature RootSignature(const PNuoCommandBuffer& commandBuffer) override;
    virtual void Draw(const PNuoCommandEncoder& encoder) override;

    void UpdatePrivateBuffer(const PNuoCommandBuffer& commandBuffer, bool selected);

};


NotationLightMesh::NotationLightMesh(const PNuoCommandBuffer& commandBuffer)
{
    PNuoDevice device = commandBuffer->CommandQueue()->Device();
    _characterUniformBuffer = device->CreatePrivateBuffer(sizeof(NuoModelCharacterUniforms));
}


PNuoRootSignature NotationLightMesh::RootSignature(const PNuoCommandBuffer& commandBuffer)
{
    PNuoRootSignature signature = NuoMeshSimple::RootSignature(commandBuffer);

    signature->AddRootConstantBuffer(3, 0, D3D12_SHADER_VISIBILITY_VERTEX);	// mesh character (e.g. transparency)
    return signature;
}


void NotationLightMesh::Draw(const PNuoCommandEncoder& encoder)
{
    encoder->SetRootConstantBuffer(3, _characterUniformBuffer);
    NuoMeshSimple::Draw(encoder);
}


void NotationLightMesh::UpdatePrivateBuffer(const PNuoCommandBuffer& commandBuffer, bool selected)
{
    NuoModelCharacterUniforms uniforms;
    uniforms.opacity = selected ? 1.0f : 0.1f;

    //[NuoMesh updatePrivateBuffer:_characterUniformBuffer withCommandQueue:self.commandQueue
    //                    withData:&uniforms withSize:sizeof(NuoModelCharacterUniforms)];

    PNuoDevice device = commandBuffer->CommandQueue()->Device();

    auto intermediate = device->CreateBuffer(&uniforms, sizeof(NuoModelCharacterUniforms));
    //_characterUniformBuffer = device->CreatePrivateBuffer(sizeof(NuoModelCharacterUniforms));
    //intermediatePool.push_back(intermediate);
    commandBuffer->CopyResource(intermediate, _characterUniformBuffer);
}






/*
@interface NotationLight()


@property (nonatomic, strong) id<MTLBuffer> characterUniformBuffer;
@property (nonatomic, weak) id<MTLCommandQueue> commandQueue;

@property (nonatomic, strong) NuoMesh* lightVector;


@end



@implementation NotationLight*/


NotationLight::NotationLight(const PNuoCommandBuffer& commandBuffer,
                             unsigned int frameCount,
                             std::vector<PNuoResource>& intermediate,
                             DXGI_FORMAT format, bool bold)
{
    // _commandQueue = commandQueue;
        
    this->UpdatePrivateUniform(commandBuffer);

    const float bodyLength = bold ? 1.2f : 1.0f;
    const float bodyRadius = bold ? 0.24f : 0.2f;
    const float headLength = bold ? 1.2f : 1.0f;
    const float headRadius = bold ? 0.36f : 0.3f;
        
    PNuoModelArrow arrow = std::make_shared<NuoModelArrow>(bodyLength, bodyRadius, headLength, headRadius);
    arrow->CreateBuffer();
        
    NuoMeshBounds meshBounds;
    meshBounds.boundingBox = arrow->GetBoundingBox();
    meshBounds.boundingSphere = NuoSphere();
    
    _lightVector->Init(commandBuffer, frameCount, intermediate, arrow, format);

    // if no MSAA, shoud uncomment the following line
    // pipelineDesc.sampleCount = 1;
    
    _lightVector->SetBoundsLocal(meshBounds);
    _lightVector->MakePipelineState(commandBuffer);

    // TODO: depth stencil state is not a renderpass state, but a pipeline state
    //       need to implement later for transparency
    //[_lightVector makeDepthStencilState];
}


/*
- (NuoMeshBounds)bounds
{
    return [_lightVector worldBounds:NuoMatrixFloat44Identity];
}


void makeResources
{
    _characterUniformBuffer = [self.commandQueue.device newBufferWithLength:sizeof(NuoModelCharacterUniforms)
                                                                    options:MTLResourceStorageModePrivate];
    [self updatePrivateUniform];
}


/
- (void)updateUniformsForView:(id<NuoRenderInFlight>)inFlight
{
    NuoLightSource* desc = _lightSourceDesc;
    const NuoBounds bounds = _lightVector.boundsLocal.boundingBox;
    
    const NuoVectorFloat3 translationToCenter
    (
        - bounds._center.x(),
        - bounds._center.y(),
        - bounds._center.z() + bounds._span.z() / 2.0f
    );
    
    const NuoMatrixFloat44 modelCenteringMatrix = NuoMatrixTranslation(translationToCenter);
    const NuoMatrixFloat44 modelMatrix = desc.lightDirection * modelCenteringMatrix;
    [_lightVector updateUniform:inFlight withTransform:modelMatrix._m];
    
    NuoModelCharacterUniforms characters;
    characters.opacity = _selected ? 1.0f : 0.1f;
}*/


void NotationLight::UpdatePrivateUniform(const PNuoCommandBuffer& commandBuffer)
{
    _lightVector->UpdatePrivateBuffer(commandBuffer, _selected);
}


/*
- (void)setSelected:(BOOL)selected
{
    BOOL changed = (_selected != selected);
    
    _selected = selected;
    
    if (changed)
        [self updatePrivateUniform];
    
    [_lightVector setTransparency:!_selected];
    [_lightVector makeDepthStencilState];
}*/


NuoPoint<float> NotationLight::HeadPointProjectedWithView(const NuoMatrixFloat44& view)
{
    const NuoVectorFloat4 startVec(0, 0, 1, 1);
    const NuoVectorFloat4 projected = view * _lightSourceDesc._lightDirection * startVec;
    
    return NuoPoint<float>(projected.x() / projected.w(), projected.y() / projected.w());
}


void NotationLight::DrawWithRenderPass(const PNuoCommandEncoder& renderPass)
{
    _lightVector->Draw(renderPass);
}
/*
- (void)drawWithRenderPass:(NuoRenderPassEncoder*)renderPass
{
    [renderPass pushParameterState:@"NotationLight"];
    
    [self updateUniformsForView:renderPass];
    [renderPass setFragmentBuffer:self.characterUniformBuffer offset:0 atIndex:1];
    
    // the light vector notation does not have varying uniform,
    // use only the 0th buffer
    //
    [_lightVector drawMesh:renderPass];
    
    [renderPass popParameterState];
}



@end

*/