

#define __SHADER_REGISTERS__ 1

#include "NuoUniforms.h"
#include "NuoMeshSimple.h"


NuoMeshSimpleVertexShaderOutput main(NuoMeshSimpleItem v)
{
    NuoMeshSimpleVertexShaderOutput outVertex;

    float4 meshPosition = mul(meshUniforms.transform, v._position);
    float4 eyePosition = mul(uniforms.viewMatrixInverse, float4(0.0, 0.0, 0.0, 1.0));

    outVertex._position = mul(uniforms.viewProjectionMatrix, meshPosition);
    outVertex._eye = eyePosition.xyz - meshPosition.xyz;

    matrix normalMatrix = meshUniforms.normalTransform;
    normalMatrix[3] = float4(0, 0, 0, 1);
    outVertex._normal = mul(normalMatrix, v._normal);

    return outVertex;
}
