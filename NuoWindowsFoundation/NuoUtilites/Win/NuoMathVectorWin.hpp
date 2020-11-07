//
//  NuoMathVectorMac.hpp
//  ModelViewer
//
//  Created by Dong on 5/12/18.
//  Copyright © 2018 middleware. All rights reserved.
//

#ifndef NuoMathVectorFunctions_h
#define NuoMathVectorFunctions_h


#include <DirectXMath.h>



template <class itemType, int itemCount>
inline typename NuoVector<itemType, itemCount>::_typeTrait::_vectorType
NuoVector<itemType, itemCount>::Normalize(const typename NuoVector<itemType, itemCount>::_typeTrait::_vectorType& v)
{
    return NuoXMNormalize(v);
}

template <>
inline float NuoVector<float, 4>::x() const
{
    DirectX::XMVECTORF32* r = (DirectX::XMVECTORF32*) & _vector;
    return r->f[0];
}

template <>
inline void NuoVector<float, 4>::x(float x)
{
    DirectX::XMVECTORF32* r = (DirectX::XMVECTORF32*) & _vector;
    r->f[0] = x;
}

template <>
inline float NuoVector<float, 4>::y() const
{
    DirectX::XMVECTORF32* r = (DirectX::XMVECTORF32*) & _vector;
    return r->f[1];
}

template <>
inline void NuoVector<float, 4>::y(float y)
{
    DirectX::XMVECTORF32* r = (DirectX::XMVECTORF32*) & _vector;
    r->f[1] = y;
}

template <>
inline float NuoVector<float, 4>::z() const
{
    DirectX::XMVECTORF32* r = (DirectX::XMVECTORF32*) & _vector;
    return r->f[2];
}

template <>
inline void NuoVector<float, 4>::z(float z)
{
    DirectX::XMVECTORF32* r = (DirectX::XMVECTORF32*) & _vector;
    r->f[2] = z;
}

template <>
inline float NuoVector<float, 4>::w() const
{
    DirectX::XMVECTORF32* r = (DirectX::XMVECTORF32*) & _vector;
    return r->f[3];
}

template <>
inline void NuoVector<float, 4>::w(float w)
{
    DirectX::XMVECTORF32* r = (DirectX::XMVECTORF32*) & _vector;
    r->f[3] = w;
}

template <>
inline NuoVector<float, 4>::NuoVector(float x, float y, float z, float w)
{
    DirectX::XMVECTORF32* r = (DirectX::XMVECTORF32*) &_vector;

    r->f[0] = x;
    r->f[1] = y;
    r->f[2] = z;
    r->f[3] = w;
}


template <class itemType, int itemCount>
inline NuoVector<itemType, itemCount> NuoVector<itemType, itemCount>::operator - () const
{
    return NuoVector<itemType, itemCount>(-(_vector));
}


template <class itemType, int dimension>
inline NuoVector<itemType, dimension>
operator * (const NuoMatrix<itemType, dimension>& m, const NuoVector<itemType, dimension>& v);


template <class itemType, int dimension>
inline NuoMatrix<itemType, dimension>
operator * (const NuoMatrix<itemType, dimension>& m1, const NuoMatrix<itemType, dimension>& m2);


template <>
inline NuoVector<float, 4>
operator * (const NuoMatrix<float, 4>& m, const NuoVector<float, 4>& v)
{
    return m._m * v._vector;
}

template <int itemCount>
inline float NuoDistance(const NuoVector<float, itemCount>& v1, const NuoVector<float, itemCount>& v2)
{
    NuoVector<float, itemCount> vector = v1 - v2;
    return NuoTraitLength(vector._vector);
}

template <int itemCount>
inline NuoVector<float, itemCount>
operator / (const NuoVector<float, itemCount>& v, float div)
{
    return NuoVector<float, itemCount>(v._vector / div);
}

template <int itemCount>
inline float NuoDot(const NuoVector<float, itemCount>& v1, const NuoVector<float, itemCount>& v2)
{
    return NuoXMDot(v1._vector, v2._vector);
}

template <int itemCount>
inline NuoVector<float, itemCount>
NuoCross(const NuoVector<float, itemCount>& v1, const NuoVector<float, itemCount>& v2)
{
    return NuoXMCross(v1._vector, v2._vector);
}

template <int itemCount>
inline NuoVector<float, itemCount>
operator * (const NuoVector<float, itemCount>& v, float mul)
{
    return NuoVector<float, itemCount>(v._vector * mul);
}


template <>
inline NuoMatrix<float, 4>::NuoMatrix()
{
    _m = DirectX::XMMatrixIdentity();
}

template <>
inline NuoMatrix<float, 3>::NuoMatrix()
{
    _m = DirectX::XMFLOAT3X3(1, 0, 0,
                             0, 1, 0,
                             0, 0, 1);
}


template <>
inline NuoMatrix<float, 4> NuoMatrix<float, 4>::Inverse() const
{
    return DirectX::XMMatrixInverse(nullptr, _m);
}


template <>
inline bool NuoMatrix<float, 4>::IsIdentity() const
{
   return *this == NuoMatrix<float, 4>::NuoMatrix();
}


template <>
inline NuoMatrix<float, 4>
operator * (const NuoMatrix<float, 4>& m1, const NuoMatrix<float, 4>& m2)
{
    return DirectX::XMMatrixMultiply(m1._m, m2._m);
}


inline NuoMatrix<float, 3> NuoMatrixExtractLinear(const NuoMatrix<float, 4>& m)
{
    DirectX::XMVECTORF32 X; X.v = m._m.r[0];
    DirectX::XMVECTORF32 Y; Y.v = m._m.r[1];
    DirectX::XMVECTORF32 Z; Z.v = m._m.r[2];

    return DirectX::XMFLOAT3X3(X.f[0], X.f[1], X.f[2],
                               Y.f[0], Y.f[1], Y.f[2],
                               Z.f[0], Z.f[1], Z.f[2]);
}


template <>
inline typename NuoMatrix<float, 4>::_typeTrait::_vectorType& NuoMatrix<float, 4>::operator[] (size_t i)
{
    return _m.r[i];
}


template <>
inline typename NuoMatrix<float, 4>::_typeTrait::_vectorType NuoMatrix<float, 4>::operator[] (size_t i) const
{
    return _m.r[i];
}


#include <stdio.h>


static inline NuoMatrix<float, 4> ToMatrix(DirectX::XMMATRIX& mat)
{
    return NuoMatrix<float, 4>(mat);
}


template <class itemType, int dimension>
inline bool
operator == (const NuoMatrix<itemType, dimension>& m1, const NuoMatrix<itemType, dimension>& m2)
{
    const DirectX::XMMATRIX& xm1 = m1._m;
    const DirectX::XMMATRIX& xm2 = m2._m;

    for (size_t i = 0; i < 4; ++i)
    {
        const DirectX::XMVECTOR& v = xm1.r[i];
        if (!DirectX::XMVector4Equal(v, xm2.r[i]))
            return false;
    }

    return true;
}



typedef NuoVector<float, 4> NuoVectorFloat4;
typedef NuoVector<float, 3> NuoVectorFloat3;
typedef NuoVector<float, 2> NuoVectorFloat2;

typedef NuoMatrix<float, 3> NuoMatrixFloat33;
typedef NuoMatrix<float, 4> NuoMatrixFloat44;


#endif /* NuoMathVectorMac_h */
