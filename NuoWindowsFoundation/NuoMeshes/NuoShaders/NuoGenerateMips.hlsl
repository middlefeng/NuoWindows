





SamplerState Sampler       : register(s0);
Texture2D<float4> SrcMip   : register(t0);
RWTexture2D<float4> OutMip : register(u0);

float4 Mip(uint2 coord)
{
    float2 uv = (coord.xy + 0.5) * InvOutTexelSize;
    return SrcMip.SampleLevel(Sampler, uv, SrcMipIndex);
}

[numthreads(8, 8, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    OutMip[id.xy] = Mip(id.xy);
}
