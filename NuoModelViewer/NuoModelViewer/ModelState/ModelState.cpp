//
//  ModelState.cpp
//  ModelViewer
//
//  Created by Dong Feng on 12/6/20.
//  Copyright © 2020 Dong Feng. All rights reserved.
//

#include "ModelState.h"

#include "NuoMeshes/NuoMesh.h"
#include "NuoMeshes/NuoMeshSceneRoot.h"

#include "NuoDirect/NuoCommandBuffer.h"


ModelState::ModelState(const PNuoCommandQueue& commandQueue, DXGI_FORMAT format)
	: _commandQueue(commandQueue),
      _format(format),
      _transMode(kTransformMode_Model)
{
    _meshOptions = {};
    _sceneRoot = std::make_shared<NuoMeshSceneRoot>();
}


void ModelState::LoadMesh(const std::string& path, NuoModelLoaderProgress progress)
{
    PNuoModelLoader loader = std::make_shared<NuoModelLoader>();
    loader->LoadModel(path);

    _modelLoader = std::make_shared<NuoModelLoaderGPU>(loader, _format);

    CreateMeshes([&progress](float progressPercent)
        {
            progress(progressPercent * (1 - 0.3f) + 0.3f);
        });

    _mainModelMesh->CenterMesh();

    // move model from camera for a default distance (3 times of r)
    //
    const NuoBounds bounds = _mainModelMesh->WorldBounds(NuoMatrixFloat44Identity).boundingBox;
    const float radius = bounds.MaxDimension() / 2.0f;
    const float defaultDistance = -3.0f * radius;
    const NuoVectorFloat3 defaultDistanceVec(0, 0, defaultDistance);
    _mainModelMesh->SetTransformTranslate(NuoMatrixTranslation(defaultDistanceVec));
}


void ModelState::SetOptions(const NuoMeshOptions& options)
{
    _meshOptions = options;
}


PNuoMeshSceneRoot ModelState::SceneRoot() const
{
    return _sceneRoot;
}


void ModelState::SetDocumentName(const std::string& name)
{
    _documentName = name;
}


std::string ModelState::DocumentName() const
{
    return _documentName;
}


void ModelState::Rotate(float x, float y)
{
    const NuoMatrixFloat44 matrix = NuoMatrixRotationAppend(_selectedMesh->TransformPoise(), x, y);

    _selectedMesh->SetTransformPoise(matrix);
}


void ModelState::Translate(const NuoVectorFloat3& translation)
{
    if (_transMode == kTransformMode_View)
    {
        _viewTranslation = NuoMatrixTranslation(translation) * _viewTranslation;
    }
    else
    {
        const NuoMatrixFloat44 transMatrix = NuoMatrixTranslation(translation) * _selectedMesh->TransformTranslate();
        _selectedMesh->SetTransformTranslate(transMatrix);
    }
}


void ModelState::CreateMeshes(NuoModelLoaderProgress progress)
{
    PNuoMesh mesh = _modelLoader->CreateMesh(_meshOptions, _commandQueue, progress);
    _sceneRoot->ReplaceMesh(_mainModelMesh, mesh);

    _mainModelMesh = std::dynamic_pointer_cast<NuoMeshCompound>(mesh);
    _selectedMesh = mesh;
}

