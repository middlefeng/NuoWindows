struct ModelViewProjection
{
    matrix MVP;
};

struct InputParamType
{
    float4 color;
};

ConstantBuffer<ModelViewProjection> ModelViewProjectionCB : register(b0);
ConstantBuffer<InputParamType> InputParam : register(b1);


struct VertexPosColor
{
    float4 Position : POSITION;
};

struct VertexShaderOutput
{
	float4 Color    : COLOR;
    float4 Position : SV_Position;
};

VertexShaderOutput main(VertexPosColor IN)
{
    VertexShaderOutput OUT;

    OUT.Position = mul(ModelViewProjectionCB.MVP, IN.Position);
    OUT.Color = float4(1.0, 0.0, 0.0, 1.0f);

    return OUT;
}