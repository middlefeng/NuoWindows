

#ifndef __MESH_SHADERS_COMMON_H__
#define __MESH_SHADERS_COMMON_H__

#include "NuoUniforms.h"


inline float3 specular_common(float3 materialSpecularColor, float materialSpecularPower,
                              NuoLightParameterUniformField lightParams,
                              float3 normal, float3 halfway, float cosTheta)
{
    float cosNHPower = pow(saturate(dot(normal, halfway)), materialSpecularPower);
    return 0.5 * cosNHPower * cosTheta * lightParams.irradiance;
}


#endif
