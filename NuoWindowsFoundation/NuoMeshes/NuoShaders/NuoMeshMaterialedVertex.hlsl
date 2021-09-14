

#define __SHADER_REGISTERS__ 1

#include "NuoUniforms.h"
#include "NuoMeshMaterialed.h"



NuoMaterialedBasicVertexOutput main(NuoMaterialedBasicItem v)
{
    NuoMaterialedBasicVertexOutput outVertex;

    float4 meshPosition = mul(meshUniforms.transform, v._position);

    outVertex._position = mul(uniforms.viewProjectionMatrix, meshPosition);
    outVertex._diffuse = v._diffuse;
    outVertex._shinessDisolveIllum = v._shinessDisolveIllum;

    matrix normalMatrix = meshUniforms.normalTransform;
    normalMatrix[3] = float4(0, 0, 0, 0);
    outVertex._normal = mul(normalMatrix, v._normal);

    return outVertex;
}
