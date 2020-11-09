

struct Light
{
    float4 direction;
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
};

ConstantBuffer<Light> light : register(b2);

struct PixelShaderInput
{
	float4 Color    : COLOR;
    float4 Normal   : NORMAL;
    float4 Position : SV_Position;
};

float4 main(PixelShaderInput IN) : SV_Target
{
    float3 normal = normalize(IN.Normal.xyz);
    float diffuseIntensity = saturate(dot(normal, light.direction));
    float3 diffuseTerm = light.diffuseColor * IN.Color * diffuseIntensity;

    return float4(diffuseTerm, 1.0);
    //return float4(1.0, 0.0, 0.0, 1.0);
}