//
//  ModelState.h
//  ModelViewer
//
//  Created by Dong Feng on 12/6/20.
//  Copyright � 2020 Dong Feng. All rights reserved.
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



typedef enum
{
	kTransformMode_Model,
	kTransformMode_View,
}
TransformMode;




class ModelState
{
	std::string _documentName;

	PNuoMeshSceneRoot _sceneRoot;
	PNuoMeshCompound _mainModelMesh;
	PNuoMesh _selectedMesh;

	PNuoCommandQueue _commandQueue;
	DXGI_FORMAT _format;

	NuoMeshOptions _meshOptions;
	PNuoModelLoaderGPU _modelLoader;

	TransformMode _transMode;

	// transform data. "viewRotation" is relative to the scene's center
	//
	NuoMatrixFloat44 _viewRotation;
	NuoMatrixFloat44 _viewTranslation;

	// need store the center of a snapshot of the scene as the meshes in the scene
	// keep moving
	//
	NuoVectorFloat3 _sceneCenter;

public:

	ModelState(const PNuoCommandQueue& commandQueue, DXGI_FORMAT format);

	void LoadMesh(const std::string& path, NuoModelLoaderProgress progress);
	void SetOptions(const NuoMeshOptions& options);

	PNuoMeshSceneRoot SceneRoot() const;

	void SetDocumentName(const std::string& name);
	std::string DocumentName() const;

	NuoBounds SelectedMeshBounds(const NuoMatrixFloat44& viewMatrix);

	void Rotate(float x, float y);
	void Translate(const NuoVectorFloat3& translate);

	NuoMatrixFloat44 ViewMatrix() const;

private:

	void CreateMeshes(NuoModelLoaderProgress progress);

};

#endif

