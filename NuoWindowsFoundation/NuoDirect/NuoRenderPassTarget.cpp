//
//  NuoRenderPassTarget.cpp
//  ModelViewer
//
//  Created by Dong Feng on 4/1/2021.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#include "NuoRenderPassTarget.h"
#include <cassert>


NuoRenderPassTarget::NuoRenderPassTarget(const PNuoDevice& device,
										 unsigned int width, unsigned int height,
										 unsigned int sampleCount, bool depthEnabled)
	: NuoRenderTarget(device, width, height, sampleCount, depthEnabled)
{
	
	
}




