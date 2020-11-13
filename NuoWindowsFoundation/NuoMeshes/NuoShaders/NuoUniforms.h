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


#endif
