

#include "NuoMeshCompound.h"


NuoMeshCompound::NuoMeshCompound(const std::vector<PNuoMesh>& meshes)
	: _meshes()
{
	SetMeshes(meshes);
}


void NuoMeshCompound::SetMeshes(const std::vector<PNuoMesh>& meshes)
{
    _meshes = meshes;

    if (!meshes.size())
        return;

    NuoBounds bounds = meshes[0]->BoundsLocal().boundingBox;
    NuoSphere sphere = meshes[0]->BoundsLocal().boundingSphere;
    for (size_t i = 1; i < meshes.size(); ++i)
    {
        bounds = bounds.Union(meshes[i]->BoundsLocal().boundingBox);
        sphere = sphere.Union(meshes[i]->BoundsLocal().boundingSphere);
    }

    NuoMeshBounds meshBounds = { bounds, sphere };
    _boundsLocal = meshBounds;
}


std::vector<D3D12_INPUT_ELEMENT_DESC> NuoMeshCompound::InputDesc()
{
	return {};
}


bool NuoMeshCompound::HasTransparency() const
{
	for (const PNuoMesh& mesh : _meshes)
	{
		if (mesh->HasTransparency())
			return true;
	}
	
	return false;
}


void NuoMeshCompound:: SetTransparency(bool transparency)
{
}


void NuoMeshCompound::Draw(const PNuoCommandEncoder& encoder)
{
    for (unsigned int renderPassStep = 0; renderPassStep < 2; ++renderPassStep)
    {
        // TODO: cull mode handling

		for (PNuoMesh& mesh : _meshes)
        {
			if ((renderPassStep == 0) && !mesh->HasTransparency() ||
				(renderPassStep == 1) && mesh->HasTransparency())
			{
				mesh->DrawBegin(encoder, _commonFunc);
				mesh->Draw(encoder);
			}
        }
    }
}


void NuoMeshCompound::DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc func)
{
	_commonFunc = func;
}
