//
//  NuoLightSource.h
//  ModelViewer
//
//  Created by middleware on 11/19/16.
//  Copyright © 2020 middleware. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "NuoMathVector.h"


@interface NuoLightSource : NSObject

@property (nonatomic, assign) NuoMatrixFloat44 lightDirection;

@property (nonatomic, assign) float lightingIrradiance;
@property (nonatomic, assign) float lightingSpecular;

@property (nonatomic, assign) bool enableShadow;
@property (nonatomic, assign) float shadowSoften;
@property (nonatomic, assign) float shadowOccluderRadius;
@property (nonatomic, assign) float shadowBias;

@end
