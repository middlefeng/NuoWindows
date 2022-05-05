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


class NuoResourceSwapChain;
typedef std::shared_ptr<NuoResourceSwapChain> PNuoResourceSwapChain;

class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;

class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;



class ModelSceneParameters
{

	PNuoResourceSwapChain _transUniformBuffers;

public:

	ModelSceneParameters(const PNuoDevice& device);

	void UpdateUniforms(const PNuoCommandBuffer& commandBuffer);

};


#endif
