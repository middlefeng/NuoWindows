

#ifndef __NuoMeshScreenSpace_h__
#define __NuoMeshScreenSpace_h__


#include "NuoMeshShaderType.h"



struct NuoMeshScreenSpaceItem
{
    float4 _position    SHADER_SEMATIC(POSITION);
    float2 _texCoord    SHADER_SEMATIC(TEX_COORD);
};

struct NuoMeshScreenSpaceVertexShaderOutput
{
    float2 _texCoord    SHADER_SEMATIC(TEX_COORD);
    float4 _position    SHADER_SEMATIC(SV_Position);
};



#endif
