

#ifndef __NuoMeshTextured_h__
#define __NuoMeshTextured_h__


#include "NuoMeshShaderType.h"



struct NuoMeshTexturedItem
{
    float4 _position  SHADER_SEMATIC(POSITION);
    float4 _normal    SHADER_SEMATIC(NORMAL);
    float2 _texCoord  SHADER_SEMATIC(TEX_COORD);
};



#endif
