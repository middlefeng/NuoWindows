

#define __SHADER_REGISTERS__ 1

#include "NuoUniforms.h"
#include "NuoMeshSimple.h"
#include "NuoMeshShadersCommon.h"



static const Material material =
{
    float3(0.6, 0.6, 0.6),
    float3(1.0, 1.0, 1.0),
    100,
};


float4 main(NuoMeshSimpleVertexShaderOutput inVertex) : SV_Target
{
    float3 lightVector = normalize(light.lightParams[0].direction.xyz);
    float3 normal = normalize(inVertex._normal.xyz);
    float opacity = modelCharacterUniforms.opacity;

    /* for now, the notation render is the only user of the simple mesh. it uses accumulate
     * blending. in order to increase the salicency of specular, the diffuse color is scaled
     * down by the opacity, while specular remains unchanged.
     */
    float cosTheta = saturate(dot(normal, lightVector));
    float3 diffuseTerm = light.lightParams[0].irradiance * cosTheta * material.diffuseColor * opacity;

    float3 eyeDirection = normalize(inVertex._eye);
    float3 halfway = normalize(lightVector + eyeDirection);

    float3 specularTerm = specular_common(material.specularColor, material.specularPower,
                                          light.lightParams[0], normal, halfway, cosTheta);

    float3 colorForLights = diffuseTerm + specularTerm;

    return float4(colorForLights, opacity);
}
