

#include "NuoMesh.h"

#include "NuoFile.h"
#include "NuoStrings.h"
#include "NuoAppInstance.h"

#include "NuoMeshes/NuoShaders/NuoUniforms.h"
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
	return std::make_shared<NuoPipelineState>(device, PipelineFormat(), EnableDepth(), SampleCount(),
											  inputElementDescs, vertexShader, pixelShader, rootSignature);
}


PNuoRootSignature NuoMesh::RootSignature(const PNuoCommandBuffer& commandBuffer)
{
	PNuoDevice device = commandBuffer->CommandQueue()->Device();

	PNuoRootSignature rootSignature = std::make_shared<NuoRootSignature>(device,
																		 D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	rootSignature->AddConstant(sizeof(NuoUniforms), 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature->AddRootConstantBuffer(1, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	return rootSignature;
}


DXGI_FORMAT NuoMesh::PipelineFormat()
{
	return _format;
}


PNuoPipelineState NuoMesh::PipelineState()
{
	return _pipelineState;
}


unsigned int NuoMesh::SampleCount()
{
	return _sampleCount;
}


bool NuoMesh::EnableDepth()
{
	return true;
}


void NuoMesh::Draw(const PNuoCommandEncoder& encoder)
{
	encoder->SetVertexBuffer(_vertexBuffer);
	encoder->DrawIndexed(_vertexBuffer->IndiciesCount());
}


void NuoMesh::DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc func)
{
	encoder->SetPipeline(PipelineState());
	func(encoder.get());
}


void NuoMeshSimple::Init(const PNuoCommandBuffer& commandBuffer,
						 std::vector<PNuoResource>& intermediate,
						 const PNuoModelSimple& model,
						 DXGI_FORMAT format, unsigned int sampleCount)
{
	NuoMeshBase<NuoMeshSimpleItem>::Init(commandBuffer, intermediate,
										 (NuoMeshSimpleItem*)model->Ptr(),
										 model->GetVerticesNumber(),
										 model->IndicesPtr(),
										 model->IndicesCount());

	_format = format;
	_sampleCount = sampleCount;
	_pipelineState = MakePipelineState(commandBuffer, "NuoMeshSimpleVertex", "NuoMeshSimplePixel");
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
	return NuoMesh::RootSignature(commandBuffer);
}


