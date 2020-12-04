

#include "NuoMeshCompound.h"


NuoMeshCompound::NuoMeshCompound(const std::vector<PNuoMesh>& meshes)
	: _meshes(meshes)
{
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
