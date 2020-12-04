
#include "NuoUniforms.h"
#include "NuoMeshMaterialed.h"



ConstantBuffer<NuoUniforms> viewProjection : register(b0);


NuoMaterialedBasicVertexOutput main(NuoMaterialedBasicItem v)
{
    NuoMaterialedBasicVertexOutput outVertex;

    outVertex._position = mul(viewProjection.viewProjectionMatrix, v._position);
    outVertex._diffuse = v._diffuse;
    outVertex._shinessDisolveIllum = v._shinessDisolveIllum;

    matrix normalMatrix = viewProjection.viewMatrix;
    normalMatrix[3] = float4(0, 0, 0, 1);
    outVertex._normal = mul(normalMatrix, v._normal);

    return outVertex;
}
