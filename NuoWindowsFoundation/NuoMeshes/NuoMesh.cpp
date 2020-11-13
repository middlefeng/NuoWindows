

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


void NuoMesh::DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc& func)
{
	encoder->SetPipeline(PipelineState());
	func(encoder.get());
}


void NuoMeshSimple::Init(const PNuoCommandBuffer& commandBuffer,
						 std::vector<PNuoResource>& intermediate,
						 const PNuoModelSimple& model)
{
	NuoMeshBase<NuoMeshSimpleItem>::Init(commandBuffer, intermediate,
										 (NuoMeshSimpleItem*)model->Ptr(),
										 model->GetVerticesNumber(),
										 model->IndicesPtr(),
										 model->IndicesCount());
}