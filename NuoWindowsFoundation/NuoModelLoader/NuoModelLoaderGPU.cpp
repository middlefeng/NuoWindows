//
//  NuoModelLoaderGPU.m
//  ModelViewer
//
//  Created by middleware on 11/29/20.
//  Copyright © 2020 middleware. All rights reserved.
//

#include "NuoModelLoaderGPU.h"


#include "NuoMeshes/NuoMeshCompound.h"
#include "NuoMeshes/NuoMeshBounds.h"



NuoModelLoaderGPU::NuoModelLoaderGPU(const PNuoModelLoader& loader, DXGI_FORMAT format,
                                     unsigned int frameCount, unsigned int sampleCount)
	: _loader(loader), _format(format),
      _frameCount(frameCount), _sampleCount(sampleCount)
{
}



NuoModelLoaderGPU::~NuoModelLoaderGPU()
{
}



PNuoMeshCompound NuoModelLoaderGPU::CreateMesh(const NuoMeshOptions& loadOption,
                                               const PNuoCommandBuffer& commandBuffer,
                                               NuoModelLoaderProgress progress)
{
    const float loadingPortionModelBuffer = loadOption._textured ? 0.70f : 0.85f;
    const float loadingPortionModelGPU = (1 - loadingPortionModelBuffer);

    auto progressFunc = [loadingPortionModelBuffer, progress](float progressValue)
    {
        progress(loadingPortionModelBuffer * progressValue);
    };

    std::vector<PNuoModelBase> models = _loader->CreateMeshWithOptions(loadOption,
                                                                       progressFunc);

    std::vector<PNuoMesh> result;
    size_t index = 0;
    for (auto& model : models)
    {
        PNuoMesh mesh = ::CreateMesh(loadOption, commandBuffer, model, _frameCount,
                                     _format, _sampleCount, _intermediates);

        NuoMeshBounds bounds;
        bounds.boundingBox = model->GetBoundingBox();

        mesh->SetBoundsLocal(bounds);
        result.push_back(mesh);

        if (progress)
            progress(++index / (float)models.size() * loadingPortionModelGPU + loadingPortionModelBuffer);
    }

    PNuoMeshCompound resultObj = std::make_shared<NuoMeshCompound>(result);

    return resultObj;
}


void NuoModelLoaderGPU::ClearIntermediates()
{
    _intermediates.clear();
}

