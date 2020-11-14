

#ifndef __NuoMeshSimple_h__
#define __NuoMeshSimple_h__


#include "NuoMeshShaderType.h"


#if __cplusplus

struct NuoMeshSimpleItem
{
    float4 _position;
    float4 _normal;
};

#else

struct NuoMeshSimpleItem
{
    float4 _position : POSITION;
    float4 _normal   : NORMAL;
};

#endif


#endif
