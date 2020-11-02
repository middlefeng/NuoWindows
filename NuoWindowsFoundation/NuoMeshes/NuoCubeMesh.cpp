

#include "NuoCubeMesh.h"
#include "NuoAppInstance.h"
#include "NuoDirect/NuoShader.h"
#include "NuoStrings.h"


void NuoCubeMesh::Init(const PNuoCommandBuffer& commandBuffer,
					   PNuoResource& intermediate,
					   float width, float height, float depth)
{
	std::vector<NuoCubeMeshVertex> vertices =
	{
		{ DirectX::XMFLOAT4(-width / 2.0f, -height / 2.0f, -depth / 2.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-width / 2.0f,  height / 2.0f, -depth / 2.0f, 1.0f) },
		{ DirectX::XMFLOAT4( width / 2.0f,  height / 2.0f, -depth / 2.0f, 1.0f) },
		{ DirectX::XMFLOAT4( width / 2.0f, -height / 2.0f, -depth / 2.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-width / 2.0f, -height / 2.0f,  depth / 2.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-width / 2.0f,  height / 2.0f,  depth / 2.0f, 1.0f) },
		{ DirectX::XMFLOAT4( width / 2.0f,  height / 2.0f,  depth / 2.0f, 1.0f) },
		{ DirectX::XMFLOAT4( width / 2.0f, -height / 2.0f,  depth / 2.0f, 1.0f) },
	};

	NuoMeshBase<NuoCubeMeshVertex>::Init(commandBuffer, intermediate,
										 vertices.data(), vertices.size());

	std::string modulePath = RemoveLastPathComponent(NuoAppInstance::GetInstance()->ModulePath());
	std::string vertexPath = modulePath + "\\" + "NuoMeshCube_V.hlsl";
	std::string pixelPath = modulePath + "\\" + "NuoMeshCube_P.hlsl";
	PNuoShader vertexShader = std::make_shared<NuoShader>(vertexPath, "NuoMeshCube_V.hlsl", NuoShader::kNuoShader_Vertex, "main");
	PNuoShader pixelShader = std::make_shared<NuoShader>(pixelPath, "NuoMeshCube_P.hlsl", NuoShader::kNuoShader_Pixel, "main");

	PNuoDevice device = commandBuffer->CommandQueue()->Device();
	PNuoRootSignature rootSignature = std::make_shared<NuoRootSignature>(device,
																		 D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	rootSignature->AddConstant(sizeof(NuoModelViewProjection), 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	_pipelineState = std::make_shared<NuoPipelineState>(device, DXGI_FORMAT_R8G8B8A8_UNORM,
														inputElementDescs, vertexShader, pixelShader, rootSignature);
}


void NuoCubeMesh::Draw(const PNuoCommandEncoder& encoder)
{
	encoder->SetPipeline(_pipelineState);
	encoder->SetVertexBuffer(_vertexBuffer);
	encoder->DrawInstanced(_vertexBuffer->Count(), 1);
}
