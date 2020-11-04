

#include "NuoCubeMesh.h"
#include "NuoAppInstance.h"
#include "NuoDirect/NuoShader.h"
#include "NuoStrings.h"
#include "NuoFile.h"


struct InputParamType
{
	DirectX::XMFLOAT4 color;
};


void NuoCubeMesh::Init(const PNuoCommandBuffer& commandBuffer,
					   std::vector<PNuoResource>& intermediate,
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

	std::vector<UINT32> indicies =
	{
		0, 1, 2, 0, 2, 3,
		4, 6, 5, 4, 7, 6,
		4, 5, 1, 4, 1, 0,
		3, 2, 6, 3, 6, 7,
		1, 5, 6, 1, 6, 2,
		4, 0, 3, 4, 3, 7
	};

	NuoMeshBase<NuoCubeMeshVertex>::Init(commandBuffer, intermediate,
										 vertices.data(), vertices.size(),
										 indicies.data(), indicies.size());

	std::string modulePath = RemoveLastPathComponent(NuoAppInstance::GetInstance()->ModulePath());
	std::string vertexPath = modulePath + "\\" + "NuoCubeMesh_V.hlsl";
	std::string pixelPath = modulePath + "\\" + "NuoCubeMesh_P.hlsl";

	std::vector<char> vertexContent;
	NuoFile vertexSource(vertexPath);
	vertexSource.ReadTo(vertexContent);
	PNuoShader vertexShader = std::make_shared<NuoShader>(vertexContent.data(), "NuoCubeMesh_V.hlsl", NuoShader::kNuoShader_Vertex, "main");

	std::vector<char> pixelContent;
	NuoFile pixelSource(pixelPath);
	pixelSource.ReadTo(pixelContent);
	PNuoShader pixelShader = std::make_shared<NuoShader>(pixelContent.data(), "NuoCubeMesh_P.hlsl", NuoShader::kNuoShader_Pixel, "main");

	PNuoDevice device = commandBuffer->CommandQueue()->Device();
	PNuoRootSignature rootSignature = std::make_shared<NuoRootSignature>(device,
																		 D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	rootSignature->AddConstant(sizeof(NuoModelViewProjection), 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature->AddConstant(sizeof(InputParamType), 1, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	_pipelineState = std::make_shared<NuoPipelineState>(device, DXGI_FORMAT_R8G8B8A8_UNORM,
														inputElementDescs, vertexShader, pixelShader, rootSignature);
}


void NuoCubeMesh::Draw(const PNuoCommandEncoder& encoder)
{
	InputParamType param;
	param.color = { 1.0, 0.5, 0.0, 1.0 };
	encoder->SetPipeline(_pipelineState);
	encoder->SetConstant(1, sizeof(InputParamType), &param);
	encoder->SetVertexBuffer(_vertexBuffer);
	encoder->DrawIndexed(_vertexBuffer->IndiciesCount());
}


