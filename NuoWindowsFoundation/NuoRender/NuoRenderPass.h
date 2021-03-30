//
//  NuoRendererPass.h
//  ModelViewer
//
//  Created by Dong Feng on 3/29/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>


#import "NuoRenderPassTarget.h"


@class NuoRenderPassEncoder;
@class NuoCommandBuffer;


@interface NuoRenderPass : NSObject

@property (nonatomic, weak) id<MTLCommandQueue> commandQueue;

@property (nonatomic, strong) NuoRenderPassTarget* renderTarget;


- (void)setDrawableSize:(CGSize)drawableSize;
- (void)setSampleCount:(NSUInteger)sampleCount;

/**
 *  draw calls that target to their own target (e.g. shadow map texture)
 */
- (void)predrawWithCommandBuffer:(NuoCommandBuffer*)commandBuffer;

/**
 *  draw calls that target to the *_renderTarget*
 */
- (void)drawWithCommandBuffer:(NuoCommandBuffer*)commandBuffer;

- (BOOL)isPipelinePass;

- (NuoRenderPassEncoder*)retainDefaultEncoder:(NuoCommandBuffer*)commandBuffer;
- (void)releaseDefaultEncoder;


@end
