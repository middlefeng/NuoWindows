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
#include "NuoRender/NuoMeshSceneRenderPass.h"


class NuoResourceSwapChain;
typedef std::shared_ptr<NuoResourceSwapChain> PNuoResourceSwapChain;

class NuoResource;
typedef std::shared_ptr<NuoResource> PNuoResource;

class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;

class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;

class NuoMeshSceneRoot;
typedef std::shared_ptr<NuoMeshSceneRoot> PNuoMeshSceneRoot;
typedef std::weak_ptr<NuoMeshSceneRoot> WPNuoMeshSceneRoot;



class ModelSceneParameters : public NuoMeshSceneParametersProvider
{

	PNuoResourceSwapChain _transUniformBuffers;
	PNuoResource _modelCharacterUnfiromBuffer;

	WPNuoMeshSceneRoot _sceneRoot;

	float _fieldOfView;
	NuoSize _drawableSize;
	NuoMatrixFloat44 _viewMatrix;
	NuoMatrixFloat44 _projection;

public:

	ModelSceneParameters(const PNuoCommandBuffer& commandBuffer, std::vector<PNuoResource>& intermediate);

	void SetSceneRoot(const PNuoMeshSceneRoot& sceneRoot);
	
	void SetFieldOfView(float fieldOfView);
	void SetDrawableSize(const NuoSize& size);
	void SetViewMatrix(const NuoMatrixFloat44& viewMatrix);

	void UpdateUniforms(const PNuoCommandBuffer& commandBuffer);

	// interface NuoMeshSceneParametersProvider

	virtual PNuoResourceSwapChain TransUniformBuffers() override;
	virtual PNuoResource ModelCharacterUnfiromBuffer() override;

};


#endif
