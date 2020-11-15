
#include "NuoUniforms.h"
#include "NuoMeshSimple.h"


ConstantBuffer<NuoLightParameterUniformField> light : register(b1);



float4 main(NuoMeshSimpleVertexShaderOutput inVertex) : SV_Target
{
    float3 normal = normalize(inVertex._normal.xyz);
    float diffuseIntensity = saturate(dot(normal, light.direction.xyz));
    float3 diffuseTerm = light.irradiance * diffuseIntensity;

    return float4(diffuseTerm, 1.0);
}
