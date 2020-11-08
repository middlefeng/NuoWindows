

/*struct Light
{
    float3 direction;
    float3 ambientColor;
    float3 diffuseColor;
    float3 specularColor;
};

/*ConstantBuffer<Light> light  : register(b2) =
{
    { 0.13, 0.72, 0.68 },
    0.05, 0.05, 0.08,
    0.9, 0.9, 0.9,
    1, 1, 1
};*/

struct PixelShaderInput
{
	float4 Color    : COLOR;
    float4 Normal   : NORMAL;
};

float4 main(PixelShaderInput IN) : SV_Target
{
    /*float3 normal = normalize(IN.Normal.xyz);
    float diffuseIntensity = saturate(dot(normal, light.direction));
    float3 diffuseTerm = light.diffuseColor * IN.Color;

    return float4(diffuseTerm, 1.0);*/
    return IN.Color;
}