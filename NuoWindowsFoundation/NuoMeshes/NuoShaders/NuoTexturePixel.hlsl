



#include "NuoMeshScreenSpace.h"


Texture2D<float4> texSource : register(t0);
SamplerState texSampler : register(s0);


float4 main(NuoMeshScreenSpaceVertexShaderOutput inVertex) : SV_Target
{
    float4 color = texSource.Sample(texSampler, inVertex._texCoord);
    return color;
}
