

#include "NuoCubeMesh.h"
#include "NuoAppInstance.h"

#include "NuoDirect/NuoShader.h"
#include "NuoDirect/NuoResourceSwapChain.h"

#include "NuoStrings.h"
#include "NuoFile.h"

#include "NuoModelLoader/NuoModelLoader.h"




std::vector<D3D12_INPUT_ELEMENT_DESC> NuoCubeMesh::InputDesc()
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		//{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	return inputElementDescs;
}


PNuoRootSignature NuoCubeMesh::RootSignature(const PNuoCommandBuffer& commandBuffer)
{
	PNuoDevice device = commandBuffer->CommandQueue()->Device();

	PNuoRootSignature rootSignature = std::make_shared<NuoRootSignature>(device,
									  D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	rootSignature->AddConstant(sizeof(NuoModelViewProjection), 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature->AddRootConstantBuffer(1, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	return rootSignature;
}

DXGI_FORMAT NuoCubeMesh::PipelineFormat()
{
	return DXGI_FORMAT_R8G8B8A8_UNORM;
}


void NuoCubeMesh::Init(const PNuoCommandBuffer& commandBuffer,
					   std::vector<PNuoResource>& intermediate,
					   float width, float height, float depth)
{
	NuoModelBoard board(2.0, 2.0, 2.0);
	board.CreateBuffer();

	std::string path = NuoAppInstance::GetInstance()->ModulePath();
	path = RemoveLastPathComponent(path);
	path = path + "/uh60.obj";

	NuoModelLoader loader;
	loader.LoadModel(path);
	std::vector<PNuoModelBase> model = loader.CreateMeshWithOptions(NuoMeshOptions(), [](float) {});

	NuoMeshBase<NuoItemSimple>::Init(commandBuffer, intermediate,
									 (NuoItemSimple*)model[0]->Ptr(), model[0]->GetVerticesNumber(),
									 model[0]->IndicesPtr(), model[0]->IndicesCount());

	PNuoDevice device = commandBuffer->CommandQueue()->Device();
	PNuoRootSignature rootSignature = RootSignature(commandBuffer);

	_pipelineState = MakePipelineState(commandBuffer, "NuoCubeMesh_V", "NuoCubeMesh_P");

	_light = std::make_shared<NuoResourceSwapChain>(device, 3, sizeof(NuoLightParameterUniformField));
}


void NuoCubeMesh::Draw(const PNuoCommandEncoder& encoder)
{
	NuoLightParameterUniformField light;
	light.direction = NuoVectorFloat4(0.13, 0.72, 0.68, 0)._vector;
	light.irradiance = 1.0;

	_light->UpdateResource(&light, sizeof(NuoLightParameterUniformField), encoder->InFlight());
	encoder->SetRootConstantBuffer(1, _light);

	encoder->SetVertexBuffer(_vertexBuffer);
	encoder->DrawIndexed(_vertexBuffer->IndiciesCount());
}


PNuoPipelineState NuoCubeMesh::PipelineState()
{
	return _pipelineState;
}


