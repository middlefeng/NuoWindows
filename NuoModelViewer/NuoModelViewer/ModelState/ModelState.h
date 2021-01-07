//
//  ModelState.h
//  ModelViewer
//
//  Created by Dong Feng on 12/6/20.
//  Copyright © 2020 Dong Feng. All rights reserved.
//


#ifndef __MODEL_STATE_H__
#define __MODEL_STATE_H__


#include <functional>
#include <memory>

#include "NuoMeshes/NuoMeshOptions.h"
#include "NuoModelLoader/NuoModelLoaderGPU.h"



class NuoMesh;
typedef std::shared_ptr<NuoMesh> PNuoMesh;

class NuoMeshSceneRoot;
typedef std::shared_ptr<NuoMeshSceneRoot> PNuoMeshSceneRoot;

class NuoMeshCompound;
typedef std::shared_ptr<NuoMeshCompound> PNuoMeshCompound;

class NuoCommandQueue;
typedef std::shared_ptr<NuoCommandQueue> PNuoCommandQueue;


class ModelState
{

	PNuoMeshSceneRoot _sceneRoot;
	PNuoMeshCompound _mainModelMesh;
	PNuoMesh _selectedMesh;

	PNuoCommandQueue _commandQueue;
	DXGI_FORMAT _format;
	unsigned int _sampleCount;

	NuoMeshOptions _meshOptions;
	PNuoModelLoaderGPU _modelLoader;

public:

	ModelState(const PNuoCommandQueue& commandQueue,
			   DXGI_FORMAT format, unsigned int sampleCount);

	void LoadMesh(const std::string& path, NuoModelLoaderProgress progress);
	void SetOptions(const NuoMeshOptions& options);

	PNuoMeshSceneRoot SceneRoot() const;

private:

	void CreateMeshes(NuoModelLoaderProgress progress);

};

#endif

