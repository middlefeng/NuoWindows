

#ifndef __NuoMeshSimple_h__
#define __NuoMeshSimple_h__


#include "NuoMeshShaderType.h"



struct NuoMeshSimpleItem
{
    float4 _position SHADER_SEMATIC(POSITION);
    float4 _normal   SHADER_SEMATIC(NORMAL);
};

struct NuoMeshSimpleVertexShaderOutput
{
    float4 _normal    SHADER_SEMATIC(NORMAL);
    float4 _position  SHADER_SEMATIC(SV_Position);
};



#endif
