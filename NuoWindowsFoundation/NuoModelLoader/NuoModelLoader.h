//
//  NuoModelLoader.h
//  ModelViewer
//
//  Created by middleware on 8/26/16.
//  Copyright © 2016 middleware. All rights reserved.
//



#ifndef __NUO_MODEL_LOADER__
#define __NUO_MODEL_LOADER__

#include "NuoModelBase.h"
#include <functional>

#include "NuoUtilites/NuoBackgroundTask.h"


typedef std::shared_ptr<NuoModelBase> PNuoModelBase;


class NuoMeshOptions;
class NuoModelLoader_Internal;


class NuoModelLoader
{

public:
    
    NuoModelLoader();
    ~NuoModelLoader();
    
    void LoadModel(const std::string& path);

    /**
     *  Create a renderable GPU mesh. A GPU mesh consists of the continuous buffer where the vertex data
     *  is stored, the associated textures, and the associated pipeline state used for rendering.
     */
    std::vector<PNuoModelBase> CreateMeshWithOptions(const NuoMeshOptions& options, NuoTaskProgress progressFunc);
    
    
private:
    
    NuoModelLoader_Internal* _internal;
    
};


typedef std::shared_ptr<NuoModelLoader> PNuoModelLoader;


#endif
