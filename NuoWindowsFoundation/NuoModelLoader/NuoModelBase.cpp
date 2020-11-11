//
//  NuoModelBase.cpp
//  ModelViewer
//
//  Created by middleware on 8/28/16.
//  Copyright © 2016 middleware. All rights reserved.
//

#include "NuoModelBase.h"
#include <DirectXMath.h>


uint32_t* NuoModelBase::IndicesPtr()
{
    return _indices.data();
}

size_t NuoModelBase::IndicesLength()
{
    return _indices.size() * sizeof(uint32_t);
}


size_t NuoModelBase::IndicesCount()
{
    return _indices.size();
}


NuoItemSimple::NuoItemSimple()
{
    memset(&_position, sizeof(_position), 0);
    memset(&_normal, sizeof(_normal), 0);
}


bool NuoItemSimple::operator == (const NuoItemSimple& i2)
{
    return DirectX::XMVector4Equal(_position, i2._position) &&
           DirectX::XMVector4Equal(_normal, i2._normal);
}

NuoModelSimple::NuoModelSimple()
{
}


void NuoModelSimple::AddTexCoord(size_t sourceIndex, const std::vector<float>& texCoordBuffer)
{
}
