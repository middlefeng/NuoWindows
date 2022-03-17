//
//  NuoModelArrow.hpp
//  ModelViewer
//
//  Created by Dong Feng on 8/28/16.
//  Ported: 9/22/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#ifndef NuoModelArrow_hpp
#define NuoModelArrow_hpp


#include "NuoModelLoader/NuoModelBase.h"


class NuoModelArrow : public NuoModelSimple
{
    
    float _bodyLength;
    float _bodyRadius;
    float _headLength;
    float _headRadius;
    
    
public:
    
    NuoModelArrow(float bodyLength, float bodyRadius, float headLength, float headRadius);
    
    void CreateBuffer();

private:
    
    NuoVectorFloat4 GetMiddleSurfaceVertex(size_t index, size_t type);
    NuoVectorFloat4 GetBodyVertex(size_t index, size_t type);
    NuoVectorFloat4 GetBodyNormal(size_t index);
    NuoVectorFloat4 GetEndVertex(size_t type);
    NuoVectorFloat4 GetHeadNormal(size_t index);
    
    void CreateEndSurface();
    void CreateBodySurface();
    void CreateMiddleSurface();
    void CreateHeadSurface();
    
};


typedef std::shared_ptr<NuoModelArrow> PNuoModelArrow;



#endif /* NuoModelArrow_hpp */
