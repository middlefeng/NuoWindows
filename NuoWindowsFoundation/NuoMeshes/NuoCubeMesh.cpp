

#include "NuoCubeMesh.h"
#include "NuoAppInstance.h"

#include "NuoDirect/NuoShader.h"
#include "NuoDirect/NuoResourceSwapChain.h"

#include "NuoStrings.h"
#include "NuoFile.h"

#include "NuoModelLoader/NuoModelLoader.h"


struct InputParamType
{
	DirectX::XMFLOAT4 color;
};

struct Light
{
	DirectX::XMFLOAT4 direction;
	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT4 specularColor;
};


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
	rootSignature->AddConstant(sizeof(InputParamType), 1, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature->AddRootConstantBuffer(2, 0, D3D12_SHADER_VISIBILITY_PIXEL);

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

	//NuoMeshBase<NuoItemSimple>::Init(commandBuffer, intermediate,
	//								 (NuoItemSimple*)board.Ptr(), board.GetVerticesNumber(),
	//								 board.IndicesPtr(), board.IndicesCount());

	NuoMeshBase<NuoItemSimple>::Init(commandBuffer, intermediate,
									 (NuoItemSimple*)model[0]->Ptr(), model[0]->GetVerticesNumber(),
									 model[0]->IndicesPtr(), model[0]->IndicesCount());

	/*std::string modulePath = RemoveLastPathComponent(NuoAppInstance::GetInstance()->ModulePath());
	std::string vertexPath = modulePath + "\\" + "NuoCubeMesh_V.hlsl";
	std::string pixelPath = modulePath + "\\" + "NuoCubeMesh_P.hlsl";

	std::vector<char> vertexContent;
	NuoFile vertexSource(vertexPath);
	vertexSource.ReadTo(vertexContent);
	PNuoShader vertexShader = std::make_shared<NuoShader>(vertexContent.data(), "NuoCubeMesh_V.hlsl", NuoShader::kNuoShader_Vertex, "main");

	std::vector<char> pixelContent;
	NuoFile pixelSource(pixelPath);
	pixelSource.ReadTo(pixelContent);
	PNuoShader pixelShader = std::make_shared<NuoShader>(pixelContent.data(), "NuoCubeMesh_P.hlsl", NuoShader::kNuoShader_Pixel, "main");*/

	PNuoDevice device = commandBuffer->CommandQueue()->Device();
	PNuoRootSignature rootSignature = RootSignature(commandBuffer);

	//std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs = InputDesc();

	//_pipelineState = std::make_shared<NuoPipelineState>(device, DXGI_FORMAT_R8G8B8A8_UNORM,
	//													inputElementDescs, vertexShader, pixelShader, rootSignature);
	_pipelineState = MakePipelineState(commandBuffer, "NuoCubeMesh_V", "NuoCubeMesh_P");

	_light = std::make_shared<NuoResourceSwapChain>(device, 3, sizeof(Light));
}


void NuoCubeMesh::Draw(const PNuoCommandEncoder& encoder)
{
	InputParamType param;
	param.color = { 1.0, 0.5, 0.0, 1.0 };


	Light light =
	{
		//{1,0,0,0},
		{ 0.13, 0.72, 0.68, 0 },
		{ 0.05, 0.05, 0.05, 0 },
		{ 0.9, 0.9, 0.9, 0 },
		{ 1, 1, 1, 0 }
	};

	encoder->SetRootConstant(1, sizeof(InputParamType), &param);

	_light->UpdateResource(&light, sizeof(Light), encoder->InFlight());
	encoder->SetRootConstantBuffer(2, _light);

	encoder->SetVertexBuffer(_vertexBuffer);
	encoder->DrawIndexed(_vertexBuffer->IndiciesCount());
}


PNuoPipelineState NuoCubeMesh::PipelineState()
{
	return _pipelineState;
}


