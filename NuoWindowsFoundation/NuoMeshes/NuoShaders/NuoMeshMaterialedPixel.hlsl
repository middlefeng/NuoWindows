

#define __SHADER_REGISTERS__ 1

#include "NuoUniforms.h"
#include "NuoMeshMaterialed.h"



float3 specular_common(float3 materialSpecularColor, float materialSpecularPower,
                       NuoLightParameterUniformField lightParams,
                       float3 normal, float3 halfway, float cosTheta)
{
    float cosNHPower = pow(saturate(dot(normal, halfway)), materialSpecularPower);
    return 0.5 * cosNHPower * cosTheta * lightParams.irradiance;
}


float4 main(NuoMaterialedBasicVertexOutput inVertex) : SV_Target
{
    float3 lightVector = normalize(light.lightParams[0].direction.xyz);
    float3 normal = normalize(inVertex._normal.xyz);
    float cosTheta = saturate(dot(normal, lightVector));
    float3 diffuseTerm = light.lightParams[0].irradiance * cosTheta * inVertex._diffuse;

    float3 eyeDirection = normalize(inVertex._eye);
    float3 halfway = normalize(lightVector + eyeDirection);

    float3 specularTerm = specular_common(inVertex._specular, inVertex._shinessDisolveIllum.x,
                                          light.lightParams[0], normal, halfway, cosTheta);

    float opacity = inVertex._shinessDisolveIllum.y;
    float3 colorForLights = diffuseTerm + specularTerm;

    return float4(colorForLights, opacity);
}
