

#define __SHADER_REGISTERS__ 1

#include "NuoUniforms.h"
#include "NuoMeshSimple.h"



float4 main(NuoMeshSimpleVertexShaderOutput inVertex) : SV_Target
{
    float3 normal = normalize(inVertex._normal.xyz);
    float diffuseIntensity = saturate(dot(normal, light.lightParams[0].direction.xyz));
    float3 diffuseTerm = light.lightParams[0].irradiance * diffuseIntensity;

    return float4(diffuseTerm, 1.0);
}
