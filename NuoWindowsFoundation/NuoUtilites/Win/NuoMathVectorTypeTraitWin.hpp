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
    typedef DirectX::XMVECTOR _vectorType;
    typedef DirectX::XMMATRIX _matrixType;
};



DirectX::XMVECTOR NuoXMLoad(const DirectX::XMFLOAT3& vector);
DirectX::XMVECTOR NuoXMLoad(const DirectX::XMFLOAT4& vector);
float NuoXMDot(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);
float NuoXMDot(const DirectX::XMVECTOR& v1, const DirectX::XMVECTOR& v2);
DirectX::XMVECTOR NuoXMCross(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);
DirectX::XMVECTOR NuoXMCross(const DirectX::XMVECTOR& v1, const DirectX::XMVECTOR& v2);
DirectX::XMFLOAT3 NuoXMNormalize(const DirectX::XMFLOAT3& v);
DirectX::XMVECTOR NuoXMNormalize(const DirectX::XMVECTOR& v);


DirectX::XMFLOAT3 operator - (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2);
DirectX::XMFLOAT3 operator - (const DirectX::XMFLOAT3& v);
DirectX::XMVECTOR operator - (const DirectX::XMVECTOR& v1, const DirectX::XMVECTOR& v2);
DirectX::XMVECTOR operator - (const DirectX::XMVECTOR& v);
DirectX::XMFLOAT3 operator / (const DirectX::XMFLOAT3& v, float div);
DirectX::XMVECTOR operator / (const DirectX::XMVECTOR& v, float div);
DirectX::XMFLOAT3 operator * (const DirectX::XMFLOAT3& v, float mul);
DirectX::XMVECTOR operator * (const DirectX::XMVECTOR& v, float mul);

DirectX::XMFLOAT3 operator * (const DirectX::XMFLOAT3X3& m, const DirectX::XMFLOAT3& v);
DirectX::XMVECTOR operator * (const DirectX::XMMATRIX& m, const DirectX::XMVECTOR& v);

float NuoTraitLength(const DirectX::XMFLOAT3& vector);
float NuoTraitLength(const DirectX::XMVECTOR& vector);



#endif /* NuoMathVectorMac_h */
