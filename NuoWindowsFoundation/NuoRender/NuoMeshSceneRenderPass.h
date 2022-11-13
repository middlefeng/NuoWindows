//
//  NuoMeshSceneRenderPass.h
//  ModelViewer
//
//  Created by Dong on 9/29/17.
//  Ported: 11/13/2022
//  Copyright © 2022 middleware. All rights reserved.
//

#include "NuoRenderPipelinePass.h"



/*@class NuoShadowMapRenderer;



typedef enum
{
    kNuoSceneMask_Opaque,
    kNuoSceneMask_Translucent
}
NuoSceneMask;



@class NuoBufferSwapChain;



@protocol NuoMeshSceneParametersProvider


- (NuoBufferSwapChain*)transUniformBuffers;
- (NuoBufferSwapChain*)lightCastBuffers;
- (NuoBufferSwapChain*)lightingUniformBuffers;
- (id<MTLBuffer>)modelCharacterUnfiromBuffer;
- (BOOL)cullEnabled;

- (id<MTLTexture>)shadowMap:(uint)index withMask:(NuoSceneMask)mask;
- (id<MTLTexture>)depthMap;


@end*/



/*@interface NuoMeshSceneRenderPass : NuoRenderPipelinePass


@property (nonatomic, weak) id<NuoMeshSceneParametersProvider> paramsProvider;
@property (nonatomic, readonly) id<MTLSamplerState> shadowMapSamplerState;


- (instancetype)initWithCommandQueue:(id<MTLCommandQueue>)commandQueue;

/**
 *  the function sets up all common uniforms that are shared by all meshes.
 *  the actual value of those uniforms come from the NuoMeshSceneParametersProvider.
 *//*
- (void)setSceneBuffersTo:(NuoRenderPassEncoder*)renderPass;

/**
 *  the function set a depth map to the render pass. unlike "setSceneBuffersTo:..." which is very
 *  basic and almost always needed, this is not required by renderers which do not need a depth map
 *//*
- (void)setDepthMapTo:(NuoRenderPassEncoder*)renderPass;


@end
*/
