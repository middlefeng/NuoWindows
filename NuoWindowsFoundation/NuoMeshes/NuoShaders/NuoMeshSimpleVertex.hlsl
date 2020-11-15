
#include "NuoMeshSimple.h"



ConstantBuffer<NuoModelViewProjection> viewProjection : register(b0);


NuoMeshSimpleVertexShaderOutput main(VertexPosColor v)
{
    NuoMeshSimpleVertexShaderOutput out;

    out._position = mul(viewProjection.viewProjectionMatrix, v._position);
    OUT.Normal = mul(viewProjection.NormalMatrix, IN.Normal);
    OUT.Color = float4(1.0, 1.0, 1.0, 1.0);//IN.Color;

    return OUT;
}