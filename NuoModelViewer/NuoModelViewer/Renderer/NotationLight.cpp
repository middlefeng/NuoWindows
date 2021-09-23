//
//  NotationLight.m
//  ModelViewer
//
//  Created by middleware on 11/13/16.
//  Copyright © 2020 middleware. All rights reserved.
//

#import "NotationLight.h"
#import "NuoMesh.h"

#include "NuoMathVector.h"

#include "NuoModelArrow.h"
#include <memory>

#include "NuoUniforms.h"
#include "NuoMeshUniform.h"

#import "NuoLightSource.h"
#import "NuoMeshBounds.h"



@interface NotationLight()


@property (nonatomic, strong) id<MTLBuffer> characterUniformBuffer;
@property (nonatomic, weak) id<MTLCommandQueue> commandQueue;

@property (nonatomic, strong) NuoMesh* lightVector;


@end



@implementation NotationLight


- (instancetype)initWithCommandQueue:(id<MTLCommandQueue>)commandQueue isBold:(BOOL)bold
{
    self = [super init];
    
    if (self)
    {
        _commandQueue = commandQueue;
        
        [self makeResources];
        
        const float bodyLength = bold ? 1.2 : 1.0;
        const float bodyRadius = bold ? 0.24 : 0.2;
        const float headLength = bold ? 1.2 : 1.0;
        const float headRadius = bold ? 0.36 : 0.3;
        
        PNuoModelArrow arrow = std::make_shared<NuoModelArrow>(bodyLength, bodyRadius, headLength, headRadius);
        arrow->CreateBuffer();
        
        const NuoMeshBounds meshBounds =
        {
            .boundingBox = arrow->GetBoundingBox(),
            .boundingSphere = NuoSphere()
        };
        
        _lightVector = [[NuoMesh alloc] initWithCommandQueue:commandQueue
                                    withVerticesBuffer:arrow->Ptr() withLength:arrow->Length()
                                           withIndices:arrow->IndicesPtr() withLength:arrow->IndicesLength()];
        
        // if no MSAA, shoud uncomment the following line
        // pipelineDesc.sampleCount = 1;
        
        [_lightVector setBoundsLocal:meshBounds];
        [_lightVector makePipelineState];
        [_lightVector makeDepthStencilState];
    }
    
    return self;
}


- (NuoMeshBounds)bounds
{
    return [_lightVector worldBounds:NuoMatrixFloat44Identity];
}


- (void)makeResources
{
    _characterUniformBuffer = [self.commandQueue.device newBufferWithLength:sizeof(NuoModelCharacterUniforms)
                                                                    options:MTLResourceStorageModePrivate];
    [self updatePrivateUniform];
}


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
}


- (void)updatePrivateUniform
{
    NuoModelCharacterUniforms uniforms;
    uniforms.opacity = _selected ? 1.0f : 0.1f;
    
    [NuoMesh updatePrivateBuffer:_characterUniformBuffer withCommandQueue:self.commandQueue
                        withData:&uniforms withSize:sizeof(NuoModelCharacterUniforms)];
}


- (void)setSelected:(BOOL)selected
{
    BOOL changed = (_selected != selected);
    
    _selected = selected;
    
    if (changed)
        [self updatePrivateUniform];
    
    [_lightVector setTransparency:!_selected];
    [_lightVector makeDepthStencilState];
}


- (CGPoint)headPointProjectedWithView:(const NuoMatrixFloat44&)view
{
    NuoLightSource* desc = _lightSourceDesc;
    
    const NuoVectorFloat4 startVec(0, 0, 1, 1);
    const NuoVectorFloat4 projected = view * desc.lightDirection * startVec;
    
    return CGPointMake(projected.x() / projected.w(), projected.y() / projected.w());
}



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
