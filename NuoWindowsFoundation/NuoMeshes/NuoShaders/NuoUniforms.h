#ifndef NuoUniforms_h
#define NuoUniforms_h


#include "NuoMeshShaderType.h"


/**
 *  vertex shader uniform for transform. if a mesh does not cast shadow, all its transforms
 *  can be multiplied into one on the CPU side and put to a single uniform.
 *  example: cube (skybox) mesh
 *
 *  otherwise, the view-projection matrix has to be separated vith the model transform.
 *
 *  vertices are transformed to the camera coordinates (eye is always at (0, 0, 0)) before passed
 *  into the fragement shader. other info required by shading (e.g. normals, light vectors) are in
 *  the world coordinate (i.e. eye being transformed through the inverse of the view matrix)
 */
struct NuoUniforms
{
    matrix viewProjectionMatrix;
    matrix viewMatrix;

    /**
     *  required by eye vector. light vectors and normals are pre-view-transform, so
     *  eye vectors must be transformed by the inverse of the view transform.
     */
    matrix viewMatrixInverse;
};


/**
 *  vertex shader uniform for model transform. if a mesh does cast shadow, the
 *  view-projection matrix has to be separated vith the model transform since
 *  the same model transform need to be applied to the view project and light/shadow-map
 *  project respectively.
 */
struct NuoMeshUniforms
{
    matrix transform;
    matrix normalTransform;
};


/**
 *  vertex shader uniform to calculate interpolatable per-vertex *shadow* data.
 *  it is separated from ModelUniforms because the dependencies to the shadow map
 *  render-pass
 */
struct NuoLightVertexUniforms
{
    // enabling shadow casting for two light sources
    matrix lightCastMatrix[2];
};


struct NuoLightParameterUniformField
{
    float4 direction;
    float specular;

    /**
     *  for directional light source in real time rendering, the radiance to a point
     *  on a lit surface is of delta distribution and the rendering equation integral
     *  results in multiplied by an irradiance on a single direction
     */
    float irradiance;
};


/**
 *  fragement shader uniform to calulate lighted color
 */
struct NuoLightUniforms
{
    NuoLightParameterUniformField lightParams[4];
    // NuoShadowParameterUniformField shadowParams[2];

    float3 ambient;
};


struct NuoModelCharacterUniforms
{
    float opacity;
};


#if __SHADER_REGISTERS__

// parameters match signature defined by NuoMesh::RootSignature()

// constant shared by most of scene meshes
//
ConstantBuffer<NuoUniforms> uniforms         : register(b0);
ConstantBuffer<NuoLightUniforms> light       : register(b1);

//
// constant commonly used by scene meshes, but each has its respective value
//

ConstantBuffer<NuoMeshUniforms> meshUniforms                        : register(b2);

// ignored (i.e. always 1.0) in a model scene
// used by notation rendering, to depict things like selection
//
ConstantBuffer<NuoModelCharacterUniforms> modelCharacterUniforms    : register(b3);

#endif



#endif
