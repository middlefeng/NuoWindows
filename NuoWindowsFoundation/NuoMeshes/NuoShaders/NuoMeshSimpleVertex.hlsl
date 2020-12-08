
#include "NuoUniforms.h"
#include "NuoMeshSimple.h"



ConstantBuffer<NuoUniforms> viewProjection   : register(b0);
ConstantBuffer<NuoMeshUniforms> meshUniforms : register(b2);


NuoMeshSimpleVertexShaderOutput main(NuoMeshSimpleItem v)
{
    NuoMeshSimpleVertexShaderOutput outVertex;

    float4 meshPosition = meshUniforms.transform * v._position;

    outVertex._position = mul(viewProjection.viewProjectionMatrix, meshPosition);

    matrix normalMatrix = viewProjection.viewMatrix;
    normalMatrix[3] = float4(0, 0, 0, 1);
    outVertex._normal = mul(normalMatrix, v._normal);

    return outVertex;
}
