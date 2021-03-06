

#define __SHADER_REGISTERS__ 1

#include "NuoUniforms.h"
#include "NuoMeshMaterialed.h"



float4 main(NuoMaterialedBasicVertexOutput inVertex) : SV_Target
{
    float3 normal = normalize(inVertex._normal.xyz);
    float diffuseIntensity = saturate(dot(normal, light.lightParams[0].direction.xyz));
    float3 diffuseTerm = light.lightParams[0].irradiance * diffuseIntensity * inVertex._diffuse;

    float opacity = inVertex._shinessDisolveIllum.y;

    return float4(diffuseTerm, opacity);
}
