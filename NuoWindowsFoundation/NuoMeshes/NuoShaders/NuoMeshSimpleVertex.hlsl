
#include "NuoUniforms.h"
#include "NuoMeshSimple.h"



ConstantBuffer<NuoUniforms> viewProjection : register(b0);


NuoMeshSimpleVertexShaderOutput main(NuoMeshSimpleItem v)
{
    NuoMeshSimpleVertexShaderOutput outVertex;

    outVertex._position = mul(viewProjection.viewProjectionMatrix, v._position);

    matrix normalMatrix = viewProjection.viewMatrix;
    normalMatrix[3] = float4(0, 0, 0, 1);
    outVertex._normal = mul(normalMatrix, v._normal);

    return outVertex;
}
