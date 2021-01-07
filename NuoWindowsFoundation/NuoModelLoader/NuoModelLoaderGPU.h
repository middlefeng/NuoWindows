//
//  NuoModelLoader.h
//  ModelViewer
//
//  Created by middleware on 8/26/16.
//  Copyright © 2016 middleware. All rights reserved.
//



#ifndef __NUO_MODEL_LOADER_GPU__
#define __NUO_MODEL_LOADER_GPU__


#include "NuoModelBase.h"
#include "NuoModelLoader.h"
#include <functional>

#include <dxgi1_6.h>


typedef std::shared_ptr<NuoModelBase> PNuoModelBase;
typedef std::function<void(float)> NuoModelLoaderProgress;

class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;

class NuoMeshCompound;
typedef std::shared_ptr<NuoMeshCompound> PNuoMeshCompound;

class NuoResource;
typedef std::shared_ptr<NuoResource> PNuoResource;


class NuoModelLoaderGPU
{

    PNuoModelLoader _loader;

    DXGI_FORMAT _format;
    unsigned int _sampleCount;

    std::vector<PNuoResource> _intermediates;

public:
    
    NuoModelLoaderGPU(const PNuoModelLoader& loader, DXGI_FORMAT format, unsigned int sampleCount);
    ~NuoModelLoaderGPU();
    
    /**
     *  Create a renderable GPU mesh. A GPU mesh consists of the continuous buffer where the vertex data
     *  is stored, the associated textures, and the associated pipeline state used for rendering.
     */
    PNuoMeshCompound CreateMesh(const NuoMeshOptions& loadOption,
                                const PNuoCommandBuffer& commandBuffer,
                                NuoModelLoaderProgress progress);

    void ClearIntermediates();
    
};

typedef std::shared_ptr<NuoModelLoaderGPU> PNuoModelLoaderGPU;


#endif
