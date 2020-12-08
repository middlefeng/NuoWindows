//
//  NuoMeshRotation.cpp
//  ModelViewer
//
//  Created by Dong Feng on 2/2/17.
//  Ported to Windows 12/7/20.
//  Copyright © 2020 Dong Feng. All rights reserved.
//

#include "NuoMeshRotation.h"



NuoMeshRotation::NuoMeshRotation()
    : _transformVector(0, 0, 0),
      _axis(0, 0, 1.0), _radius(0)
{
    GenerateMatrices();
}



NuoMeshRotation::NuoMeshRotation(const NuoMeshRotation& rotation)
    : _transformVector(rotation._transformVector),
      _axis(rotation._axis),
      _radius(rotation._radius)
{
    
    GenerateMatrices();
}


void NuoMeshRotation::GenerateMatrices()
{
    NuoMatrixFloat44 transMatrix = NuoMatrixTranslation(_transformVector);
    NuoMatrixFloat44 transMatrixInv = NuoMatrixTranslation(-_transformVector);
    NuoMatrixFloat44 rotationMatrix = NuoMatrixRotation(_axis.Normalize(), _radius);
    
    _cachedResult = (transMatrixInv * (rotationMatrix * transMatrix));
}



void NuoMeshRotation::SetRadius(float radius)
{
    _radius = radius;
    GenerateMatrices();
}



const NuoMatrixFloat44& NuoMeshRotation::RotationMatrix() const
{
    return _cachedResult;
}


