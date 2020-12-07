

#include "NuoMeshSceneRoot.h"


NuoMeshSceneRoot::NuoMeshSceneRoot()
    : NuoMeshCompound({})
{
}


NuoMeshSceneRoot::NuoMeshSceneRoot(const std::vector<PNuoMesh>& meshes)
	: NuoMeshCompound(meshes)
{
}


void NuoMeshSceneRoot::ReplaceMesh(const PNuoMesh& meshOld, const PNuoMesh& replacer)
{
    bool haveReplaced = false;

    // put the main model at the end of the draw queue,
    // for now it is the only one has transparency
    //

    for (unsigned int i = 0; i < _meshes.size(); ++i)
    {
        if (_meshes[i] == meshOld)
        {
            _meshes[i] = replacer;
            haveReplaced = true;

            break;
        }
    }

    if (!haveReplaced)
        _meshes.push_back(replacer);
}

