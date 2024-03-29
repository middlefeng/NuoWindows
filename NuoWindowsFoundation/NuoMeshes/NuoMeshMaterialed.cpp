﻿

#include "NuoMeshMaterialed.h"


NuoMeshMaterialed::NuoMeshMaterialed()
	: _hasTransparency(false),
	_physicallyReflection(false)
{
}


NuoMeshMaterialed::~NuoMeshMaterialed()
{
}	


void NuoMeshMaterialed::Init(const PNuoCommandBuffer& commandBuffer,
							 unsigned int frameCount,
							 std::vector<PNuoResource>& intermediate,
							 const PNuoModelMaterialed& model,
							 DXGI_FORMAT format)
{
	_format = format;
	
	NuoMeshBase<NuoMaterialedBasicItem>::Init(commandBuffer, frameCount, intermediate,
											  (NuoMaterialedBasicItem*)model->Ptr(),
											  model->GetVerticesNumber(),
											  model->IndicesPtr(),
											  model->IndicesCount());
}



bool NuoMeshMaterialed::HasTransparency() const
{
	return _hasTransparency;
}


void NuoMeshMaterialed::SetTransparency(bool transparency)
{
	_hasTransparency = transparency;
}


void NuoMeshMaterialed::SetPhysicallyReflection(bool reflection)
{
	_physicallyReflection = reflection;
}


void NuoMeshMaterialed::MakePipelineState(const PNuoCommandBuffer& commandBuffer)
{
	_pipelineState = NuoMesh::MakePipelineState(commandBuffer, "NuoMeshMaterialedVertex", "NuoMeshMaterialedPixel");
}


std::vector<D3D12_INPUT_ELEMENT_DESC> NuoMeshMaterialed::InputDesc()
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "DIFFUSE",   0, DXGI_FORMAT_R32G32B32_FLOAT,	  0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "AMBIENT",   0, DXGI_FORMAT_R32G32B32_FLOAT,	  0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "SPECULAR",  0, DXGI_FORMAT_R32G32B32_FLOAT,	  0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "SHINESS_DISOLVE_ILLUM",  0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 68, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	return inputElementDescs;
}



