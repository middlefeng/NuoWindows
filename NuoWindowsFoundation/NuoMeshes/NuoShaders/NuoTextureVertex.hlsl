


#include "NuoMeshScreenSpace.h"



NuoMeshScreenSpaceVertexShaderOutput main(NuoMeshScreenSpaceItem v)
{
    NuoMeshScreenSpaceVertexShaderOutput outVertex;

    outVertex._position = v._position;
    outVertex._position.z = 1.0;
    outVertex._position.w = 1.0;
    outVertex._texCoord = v._texCoord;

    return outVertex;
}
