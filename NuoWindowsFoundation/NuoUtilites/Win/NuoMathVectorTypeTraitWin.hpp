//
//  NuoMathVectorWin.hpp
//  ModelViewer
//
//  Created by Dong on 11/4/2020.
//  Copyright © 2018 middleware. All rights reserved.
//

#ifndef NuoMathVectorTypeTrait_h
#define NuoMathVectorTypeTrait_h

#include <DirectXMath.h>


template <>
class VectorTrait<float, 2>
{
public:
    typedef DirectX::XMFLOAT2 _vectorType;
};


template <>
class VectorTrait<float, 3>
{
public:
    typedef DirectX::XMFLOAT3 _vectorType;
    typedef DirectX::XMFLOAT3X3 _matrixType;
};

template <>
class VectorTrait<float, 4>
{
public:
    typedef DirectX::XMFLOAT4 _vectorType;
    typedef DirectX::XMFLOAT4X4 _matrixType;
};

#endif /* NuoMathVectorMac_h */
