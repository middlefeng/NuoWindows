//
//  NuoLightSource.h
//  ModelViewer
//
//  Created by Dong Feng on 11/19/16.
//  Ported: 9/22/21
//  Copyright © 2021 Dong Feng. All rights reserved.
//


#ifndef __NUO_LIGHT_SOURCE_H__
#define __NUO_LIGHT_SOURCE_H__


#include "NuoMathVector.h"


struct NuoLightSource
{
	NuoMatrixFloat44 _lightDirection;

	float _lightingIrradiance;
	float _lightingSpecular;

	bool _enableShadow;
	float _shadowSoften;
	float _shadowOccluderRadius;
	float _shadowBias;

	NuoLightSource();
};


#endif

