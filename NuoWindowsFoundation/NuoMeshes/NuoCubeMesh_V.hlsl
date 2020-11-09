struct ModelViewProjection
{
    matrix MVP;
    matrix NormalMatrix;
};

struct InputParamType
{
    float4 color;
};

/*struct InputParamType
{
    float4 direction;
};*/

//ConstantBuffer<InputParamType> aLight : register(b1);

ConstantBuffer<ModelViewProjection> ModelViewProjectionCB : register(b0);
ConstantBuffer<InputParamType> InputParam : register(b1);


struct VertexPosColor
{
    float4 Position : POSITION;
    float4 Normal   : NORMAL;
};

struct VertexShaderOutput
{
	float4 Color    : COLOR;
    float4 Normal   : NORMAL;
    float4 Position : SV_Position;
};

VertexShaderOutput main(VertexPosColor IN)
{
    VertexShaderOutput OUT;

    OUT.Position = mul(ModelViewProjectionCB.MVP, IN.Position);
    OUT.Normal = mul(ModelViewProjectionCB.NormalMatrix, IN.Normal);
    OUT.Color = float4(1.0, 1.0, 1.0, 1.0f);

    return OUT;
}