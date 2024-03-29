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

class NuoCommandQueue;
typedef std::shared_ptr<NuoCommandQueue> PNuoCommandQueue;

class NuoMeshCompound;
typedef std::shared_ptr<NuoMeshCompound> PNuoMeshCompound;

class NuoResource;
typedef std::shared_ptr<NuoResource> PNuoResource;


class NuoModelLoaderGPU
{

    PNuoModelLoader _loader;

    DXGI_FORMAT _format;

public:
    
    NuoModelLoaderGPU(const PNuoModelLoader& loader, DXGI_FORMAT format);
    ~NuoModelLoaderGPU();
    
    /**
     *  Create a renderable GPU mesh. A GPU mesh consists of the continuous buffer where the vertex data
     *  is stored, the associated textures, and the associated pipeline state used for rendering.
     */
    PNuoMeshCompound CreateMesh(const NuoMeshOptions& loadOption,
                                const PNuoCommandQueue& commandQueue,
                                NuoModelLoaderProgress progress);

};

typedef std::shared_ptr<NuoModelLoaderGPU> PNuoModelLoaderGPU;


#endif
