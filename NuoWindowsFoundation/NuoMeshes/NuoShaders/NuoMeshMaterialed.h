

#ifndef __NuoMeshMaterialed_h__
#define __NuoMeshMaterialed_h__


#include "NuoMeshShaderType.h"



struct NuoMaterialedTexturedBasicItem
{
    float4 _position                SHADER_SEMATIC(POSITION);
    float4 _normal                  SHADER_SEMATIC(NORMAL);
    float2 _texCoord                SHADER_SEMATIC(TEX_COORD);

    float3 _diffuse                 SHADER_SEMATIC(DIFFUSE);
    float3 _ambient                 SHADER_SEMATIC(AMBIENT);
    float3 _specular                SHADER_SEMATIC(SPECULAR);
    float3 _shinessDisolveIllum     SHADER_SEMATIC(SHINESS_DISOLVE_ILLUM);
};


struct NuoMaterialedBasicItem
{
    float4 _position                SHADER_SEMATIC(POSITION);
    float4 _normal                  SHADER_SEMATIC(NORMAL);

    float3 _diffuse                 SHADER_SEMATIC(DIFFUSE);
    float3 _ambient                 SHADER_SEMATIC(AMBIENT);
    float3 _specular                SHADER_SEMATIC(SPECULAR);
    float3 _shinessDisolveIllum     SHADER_SEMATIC(SHINESS_DISOLVE_ILLUM);
};


struct NuoMaterialedBasicVertexOutput
{
    float4 _position                SHADER_SEMATIC(SV_Position);
    float4 _normal                  SHADER_SEMATIC(NORMAL);

    float3 _diffuse                 SHADER_SEMATIC(DIFFUSE);
    float3 _ambient                 SHADER_SEMATIC(AMBIENT);
    float3 _specular                SHADER_SEMATIC(SPECULAR);
    float3 _shinessDisolveIllum     SHADER_SEMATIC(SHINESS_DISOLVE_ILLUM);
};



#endif
