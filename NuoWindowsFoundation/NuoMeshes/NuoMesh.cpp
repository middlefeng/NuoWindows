

#include "NuoMesh.h"

#include "NuoFile.h"
#include "NuoStrings.h"
#include "NuoAppInstance.h"
#include "NuoModelLoader/NuoModelBase.h"


PNuoPipelineState NuoMesh::MakePipelineState(const PNuoCommandBuffer& commandBuffer,
											 const std::string& vertex, const std::string& pixel)
{
	auto appInstance = NuoAppInstance::GetInstance();
	auto path = RemoveLastPathComponent(appInstance->ModulePath()) + "/NuoShaders/";

	std::vector<char> vertexContent;
	NuoFile vertexSource(path + vertex + ".hlsl");
	vertexSource.ReadTo(vertexContent);
	PNuoShader vertexShader = std::make_shared<NuoShader>(vertexContent.data(), vertex, NuoShader::kNuoShader_Vertex, "main");

	std::vector<char> pixelContent;
	NuoFile pixelSource(path + pixel + ".hlsl");
	pixelSource.ReadTo(pixelContent);
	PNuoShader pixelShader = std::make_shared<NuoShader>(pixelContent.data(), pixel, NuoShader::kNuoShader_Pixel, "main");

	PNuoRootSignature rootSignature = RootSignature(commandBuffer);

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs = InputDesc();

	const PNuoDevice& device = commandBuffer->CommandQueue()->Device();
	return std::make_shared<NuoPipelineState>(device, PipelineFormat(),
											  inputElementDescs, vertexShader, pixelShader, rootSignature);
}


PNuoRootSignature NuoMesh::RootSignature(const PNuoCommandBuffer& commandBuffer)
{
	PNuoDevice device = commandBuffer->CommandQueue()->Device();

	PNuoRootSignature rootSignature = std::make_shared<NuoRootSignature>(device,
																		 D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	rootSignature->AddConstant(sizeof(NuoModelViewProjection), 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature->AddRootConstantBuffer(2, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	return rootSignature;
}


void NuoMesh::DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc& func)
{
	encoder->SetPipeline(PipelineState());
	func(encoder.get());
}


void NuoMeshSimple::Init(const PNuoCommandBuffer& commandBuffer,
						 std::vector<PNuoResource>& intermediate,
						 const PNuoModelSimple& model,
						 DXGI_FORMAT format)
{
	NuoMeshBase<NuoMeshSimpleItem>::Init(commandBuffer, intermediate,
										 (NuoMeshSimpleItem*)model->Ptr(),
										 model->GetVerticesNumber(),
										 model->IndicesPtr(),
										 model->IndicesCount());

	_format = format;
}


std::vector<D3D12_INPUT_ELEMENT_DESC> NuoMeshSimple::InputDesc()
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	return inputElementDescs;
}


PNuoRootSignature NuoMeshSimple::RootSignature(const PNuoCommandBuffer& commandBuffer)
{
	PNuoDevice device = commandBuffer->CommandQueue()->Device();

	PNuoRootSignature rootSignature = std::make_shared<NuoRootSignature>(device,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	rootSignature->AddConstant(sizeof(NuoModelViewProjection), 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	//rootSignature->AddConstant(sizeof(InputParamType), 1, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature->AddRootConstantBuffer(2, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	return rootSignature;
}


DXGI_FORMAT NuoMeshSimple::PipelineFormat()
{
	return _format;
}