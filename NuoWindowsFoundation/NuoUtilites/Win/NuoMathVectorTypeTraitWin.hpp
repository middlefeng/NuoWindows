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


inline DirectX::XMFLOAT3 operator - (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    DirectX::XMFLOAT3 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}


#endif /* NuoMathVectorMac_h */
