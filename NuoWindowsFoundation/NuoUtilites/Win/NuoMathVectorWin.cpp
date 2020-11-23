//
//  NuoMathVectorMac.cpp
//  ModelViewer
//
//  Created by Dong on 5/11/18.
//  Copyright © 2018 middleware. All rights reserved.
//

#include "NuoMathVector.h"

DirectX::XMFLOAT3 operator + (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    DirectX::XMFLOAT3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;

    return result;
}

DirectX::XMFLOAT3 operator - (const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    DirectX::XMFLOAT3 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}

DirectX::XMFLOAT3 operator - (const DirectX::XMFLOAT3& v)
{
    DirectX::XMFLOAT3 result;
    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;

    return result;
}

DirectX::XMVECTOR operator + (const DirectX::XMVECTOR& v1, const DirectX::XMVECTOR& v2)
{
    return DirectX::XMVectorAdd(v1, v2);
}

DirectX::XMVECTOR operator - (const DirectX::XMVECTOR& v1, const DirectX::XMVECTOR& v2)
{
    return DirectX::XMVectorSubtract(v1, v2);
}


DirectX::XMVECTOR operator - (const DirectX::XMVECTOR& v)
{
    return DirectX::XMVectorNegate(v);
}

DirectX::XMFLOAT3 operator / (const DirectX::XMFLOAT3& v, float div)
{
    DirectX::XMFLOAT3 result;
    result.x = v.x / div;
    result.y = v.y / div;
    result.z = v.z / div;

    return result;
}


DirectX::XMVECTOR operator / (const DirectX::XMVECTOR& v, float div)
{
    DirectX::XMVECTORF32 result;
    DirectX::XMVECTORF32* pv = (DirectX::XMVECTORF32*) &v;

    result.f[0] = pv->f[0] / div;
    result.f[1] = pv->f[1] / div;
    result.f[2] = pv->f[2] / div;
    result.f[3] = pv->f[3] / div;

    return result.v;
}

DirectX::XMFLOAT3 operator * (const DirectX::XMFLOAT3& v, float mul)
{
    DirectX::XMFLOAT3 result;
    result.x = v.x * mul;
    result.y = v.y * mul;
    result.z = v.z * mul;

    return result;
}

DirectX::XMVECTOR operator * (const DirectX::XMVECTOR& v, float mul)
{
    DirectX::XMVECTORF32 result;
    DirectX::XMVECTORF32* pv = (DirectX::XMVECTORF32*) &v;

    result.f[0] = pv->f[0] * mul;
    result.f[1] = pv->f[1] * mul;
    result.f[2] = pv->f[2] * mul;
    result.f[3] = pv->f[3] * mul;

    return result.v;
}

DirectX::XMFLOAT3 operator * (const DirectX::XMFLOAT3X3& m, const DirectX::XMFLOAT3& v)
{
    DirectX::XMMATRIX xm = DirectX::XMLoadFloat3x3(&m);
    DirectX::XMVECTOR r = NuoXMLoad(v);
    r = DirectX::XMVector3Transform(r, xm);

    auto pr = ((DirectX::XMVECTORF32*) &r)->f;

    return DirectX::XMFLOAT3(pr[0], pr[1], pr[2]);
}

DirectX::XMVECTOR operator * (const DirectX::XMMATRIX& m, const DirectX::XMVECTOR& v)
{
    return DirectX::XMVector4Transform(v, m);
}


DirectX::XMVECTOR NuoXMLoad(const DirectX::XMFLOAT3& vector)
{
    return DirectX::XMLoadFloat3(&vector);
}

DirectX::XMVECTOR NuoXMLoad(const DirectX::XMFLOAT4& vector)
{
    return DirectX::XMLoadFloat4(&vector);
}

float NuoXMDot(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    auto r = DirectX::XMVector3Dot(NuoXMLoad(v1), NuoXMLoad(v2));
    return ((DirectX::XMVECTORF32*) &r)->f[0];
}

float NuoXMDot(const DirectX::XMVECTOR& v1, const DirectX::XMVECTOR& v2)
{
    auto r = DirectX::XMVector4Dot(v1, v2);
    return ((DirectX::XMVECTORF32*) & r)->f[0];
}

DirectX::XMFLOAT3 NuoXMCross(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
    DirectX::XMVECTORF32 result;
    result.v = DirectX::XMVector3Cross(NuoXMLoad(v1), NuoXMLoad(v2));

    return DirectX::XMFLOAT3(result.f[0], result.f[1], result.f[2]);
}

DirectX::XMVECTOR NuoXMCross(const DirectX::XMVECTOR& v1, const DirectX::XMVECTOR& v2)
{
    return DirectX::XMVector3Cross(v1, v2);
}


DirectX::XMFLOAT3 NuoXMNormalize(const DirectX::XMFLOAT3& v)
{
    auto r = DirectX::XMVector3Normalize(NuoXMLoad(v));
    auto pr = ((DirectX::XMVECTORF32*) &r)->f;
    return DirectX::XMFLOAT3(pr[0], pr[1], pr[2]);
}


DirectX::XMVECTOR NuoXMNormalize(const DirectX::XMVECTOR& v)
{
    return DirectX::XMVector4Normalize(v);
}


float NuoTraitLength(const DirectX::XMFLOAT3& vector)
{
    DirectX::XMVECTOR v = NuoXMLoad(vector);
    DirectX::XMVECTORF32 result;
    result.v = DirectX::XMVector3Length(v);

    return result.f[0];
}

float NuoTraitLength(const DirectX::XMVECTOR& vector)
{
    DirectX::XMVECTORF32 result;
    result.v = DirectX::XMVector4Length(vector);

    return result.f[0];
}

NuoMatrix<float, 4> NuoMatrixTranslation(const NuoVector<float, 3>& t)
{
    return DirectX::XMMatrixTranslation(t.x(), t.y(), t.z());
}

NuoMatrix<float, 4> NuoMatrixRotation(const NuoVector<float, 3>& axis, float angle)
{
    DirectX::XMVECTOR v = NuoXMLoad(axis._vector);
    return DirectX::XMMatrixRotationAxis(v, angle);
}

NuoMatrix<float, 4> NuoMatrixRotation(float rotateX, float rotateY)
{
    auto mx =  DirectX::XMMatrixRotationX(rotateX);
    auto my = DirectX::XMMatrixRotationY(rotateY);

    return mx * my;
}



NuoMatrix<float, 4> NuoMatrixPerspective(float aspect, float fovy, float near, float far)
{
    // NOT use OpenGL persepctive!
    // Metal uses a 2x2x1 canonical cube (z in [0,1]), rather than the 2x2x2 one in OpenGL.

    // glm::mat4x4 gmat = glm::perspective(fovy, aspect, near, far);
    /*
     T const tanHalfFovy = tan(fovy / static_cast<T>(2));

     tmat4x4<T, defaultp> Result(static_cast<T>(0));
     Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
     Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
     Result[2][2] = - (zFar + zNear) / (zFar - zNear);
     Result[2][3] = - static_cast<T>(1);
     Result[3][2] = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
     return Result;
     */
     
#if NO_DirectX
     float yScale = 1 / tan(fovy * 0.5);
     float xScale = yScale / aspect;
     float zRange = far - near;
     float zScale = -(far) / zRange;
     float wzScale = - far * near / zRange;

     vector_float4 P = { xScale, 0, 0, 0 };
     vector_float4 Q = { 0, yScale, 0, 0 };
     vector_float4 R = { 0, 0, zScale, -1 };
     vector_float4 S = { 0, 0, wzScale, 0 };

     matrix_float4x4 mat = { P, Q, R, S };
     return mat;
#else
    return DirectX::XMMatrixPerspectiveFovRH(fovy, aspect, near, far);
#endif
}


NuoMatrix<float, 4> NuoMatrixOrthor(float left, float right, float top, float bottom, float near, float far)
{
    /* Ortho in OpenGL
     
     tmat4x4<T, defaultp> Result(1);
     Result[0][0] = static_cast<T>(2) / (right - left);
     Result[1][1] = static_cast<T>(2) / (top - bottom);
     Result[2][2] = - static_cast<T>(2) / (zFar - zNear);
     Result[3][0] = - (right + left) / (right - left);
     Result[3][1] = - (top + bottom) / (top - bottom);
     Result[3][2] = - (zFar + zNear) / (zFar - zNear);
     */
    
#if NO_DIRECTX
    // Ortho in Metal
    // http://blog.athenstean.com/post/135771439196/from-opengl-to-metal-the-projection-matrix
    
    float yScale = 2 / (top - bottom);
    float xScale = 2 / (right - left);
    float zRange = far - near;
    float zScale = - 1 / zRange;
    float wzScale = - near / zRange;
    float wyScale = - (top + bottom) / (top - bottom);
    float wxScale = - (right + left) / (right - left);
    
    vector_float4 P = { xScale, 0, 0, 0 };
    vector_float4 Q = { 0, yScale, 0, 0 };
    vector_float4 R = { 0, 0, zScale, 0 };
    vector_float4 S = { wxScale, wyScale, wzScale, 1 };
    
    matrix_float4x4 mat = { P, Q, R, S };
    return mat;
#else
    DirectX::XMMATRIX projection = DirectX::XMMatrixOrthographicRH(right - left, top - bottom, near, far);
    DirectX::XMMATRIX view = DirectX::XMMatrixTranslation(-(left + right) / 2.0, -(top + bottom) / 2.0, 0);
    return DirectX::XMMatrixMultiply(view, projection);
#endif
}
