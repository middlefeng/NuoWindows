

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


NuoMeshBounds NuoMeshCompound::WorldBounds(const NuoMatrixFloat44& transform)
{
    if (!_meshes.size())
        return { NuoBounds(), NuoSphere() };

    NuoMatrixFloat44 transformLocal = _transformTranslate * _transformPoise;
    NuoMatrixFloat44 transformWorld = transform * transformLocal;

    NuoMeshBounds meshBounds = _meshes[0]->WorldBounds(transformWorld);
    NuoBounds bounds = meshBounds.boundingBox;
    NuoSphere sphere = meshBounds.boundingSphere;

    for (size_t i = 1; i < _meshes.size(); ++i)
    {
        NuoMeshBounds meshBoundsItem = _meshes[i]->WorldBounds(transformWorld);

        bounds = bounds.Union(meshBoundsItem.boundingBox);
        sphere = sphere.Union(meshBoundsItem.boundingSphere);
    }

    return { bounds, sphere };
}


std::vector<D3D12_INPUT_ELEMENT_DESC> NuoMeshCompound::InputDesc()
{
	return {};
}


void NuoMeshCompound::MakePipelineState(const PNuoCommandBuffer& commandBuffer)
{
	for (const PNuoMesh& mesh : _meshes)
	{
		mesh->MakePipelineState(commandBuffer);
	}
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


void NuoMeshCompound::UpdateUniform(unsigned int inFlight, const NuoMatrixFloat44& transform)
{
	const NuoMatrixFloat44 transformLocal = _transformTranslate * _transformPoise;
	const NuoMatrixFloat44 transformWorld = transform * transformLocal;

	for (PNuoMesh& mesh : _meshes)
		mesh->UpdateUniform(inFlight, transformWorld);
}


void NuoMeshCompound::DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc func)
{
	_commonFunc = func;
}
