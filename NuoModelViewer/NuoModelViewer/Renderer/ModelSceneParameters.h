//
//  ModelSceneParameters.h
//  ModelViewer
//
//  Created by Dong on 7/26/19.
//  Ported 5/4/2022
//  Copyright © 2022 Dong Feng. All rights reserved.
//


#ifndef __MODEL_SCENE_PARAMETERS_H__
#define __MODEL_SCENE_PARAMETERS_H__

#pragma once

#include <memory>
#include <vector>

#include "NuoDirect/NuoSize.h"
#include "NuoUtilites/NuoMathVector.h"


class NuoResourceSwapChain;
typedef std::shared_ptr<NuoResourceSwapChain> PNuoResourceSwapChain;

class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;

class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;

class NuoMeshSceneRoot;
typedef std::shared_ptr<NuoMeshSceneRoot> PNuoMeshSceneRoot;
typedef std::weak_ptr<NuoMeshSceneRoot> WPNuoMeshSceneRoot;



class ModelSceneParameters
{

	PNuoResourceSwapChain _transUniformBuffers;

	WPNuoMeshSceneRoot _sceneRoot;

	float _fieldOfView;
	NuoSize _drawableSize;
	NuoMatrixFloat44 _viewMatrix;
	NuoMatrixFloat44 _projection;

public:

	ModelSceneParameters(const PNuoDevice& device);

	void SetSceneRoot(const PNuoMeshSceneRoot& sceneRoot);
	void SetDrawableSize(const NuoSize& size);
	void SetViewMatrix(const NuoMatrixFloat44& viewMatrix);

	void UpdateUniforms(const PNuoCommandBuffer& commandBuffer);

	// TODO: should be part of interface NuoMeshSceneParametersProvider

	PNuoResourceSwapChain TransUniformBuffers();

};


#endif
