

#include "NuoMeshCompound.h"


NuoMeshCompound::NuoMeshCompound(const std::vector<PNuoMesh>& meshes)
	: _meshes(meshes)
{
}


std::vector<D3D12_INPUT_ELEMENT_DESC> NuoMeshCompound::InputDesc()
{
	return {};
}


void NuoMeshCompound::Draw(const PNuoCommandEncoder& encoder)
{
	for (PNuoMesh& mesh : _meshes)
	{
		mesh->DrawBegin(encoder, _commonFunc);
		mesh->Draw(encoder);
	}
}


void NuoMeshCompound::DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc func)
{
	_commonFunc = func;
}
