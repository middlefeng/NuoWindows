
#include "NuoUniforms.h"


/*struct Light
{
    float4 direction;
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
};*/

ConstantBuffer<NuoLightParameterUniformField> light : register(b1);

struct PixelShaderInput
{
	float4 Color    : COLOR;
    float4 Normal   : NORMAL;
    float4 Position : SV_Position;
};

float4 main(PixelShaderInput IN) : SV_Target
{
    float3 normal = normalize(IN.Normal.xyz);
    float diffuseIntensity = saturate(dot(normal, light.direction.xyz));
    float3 diffuseTerm = light.irradiance * IN.Color * diffuseIntensity;

    return float4(diffuseTerm, 1.0);
}