//
//  NuoLightSource.m
//  ModelViewer
//
//  Created by middleware on 11/19/16.
//  Copyright © 2020 middleware. All rights reserved.
//

#include "NuoLightSource.h"


NuoLightSource::NuoLightSource()
	: _lightDirection(NuoMatrixFloat44Identity),
	  _lightingIrradiance(0),
	  _lightingSpecular(0),
	  _enableShadow(0),
	  _shadowSoften(0),
	  _shadowOccluderRadius(0),
	  _shadowBias(0)
{
}


