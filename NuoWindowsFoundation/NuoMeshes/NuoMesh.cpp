

#include "NuoMesh.h"

#include "NuoFile.h"
#include "NuoStrings.h"
#include "NuoAppInstance.h"

#include "NuoMeshes/NuoShaders/NuoUniforms.h"

#include "NuoModelLoader/NuoModelBase.h"
#include "NuoMeshMaterialed.h"

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

	NuoBlendingMode blendingMode = HasTransparency() ? kNuoBlending_Alpha : kNuoBlending_None;

	const PNuoDevice& device = commandBuffer->CommandQueue()->Device();
	return std::make_shared<NuoPipelineState>(device, PipelineFormat(), EnableDepth(), SampleCount(), blendingMode,
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


NuoMeshBounds NuoMesh::BoundsLocal() const
{
	return _boundsLocal;
}


void NuoMesh::SetBoundsLocal(const NuoMeshBounds& bounds)
{
	_boundsLocal = bounds;

	// calculate the sphere from box if the former is not calculated.
	// some subclass might do this by itself (such as compound mesh)
	//
	if (_boundsLocal.boundingSphere._radius == 0.)
	{
		_boundsLocal.boundingSphere = _boundsLocal.boundingBox.Sphere();
	}
}


void NuoMesh::CenterMesh()
{
	const NuoBounds& bounds = _boundsLocal.boundingBox;
	_transformPoise = NuoMatrixTranslation(-bounds._center);
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


bool NuoMeshSimple::HasTransparency() const
{
	return false;
}


void NuoMeshSimple::SetTransparency(bool transparency)
{
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




PNuoMesh CreateMesh(const NuoMeshOptions& options,
					const PNuoCommandBuffer& commandBuffer,
					const PNuoModelBase& model,
					DXGI_FORMAT format, unsigned int sampleCount,
					std::vector<PNuoResource>& intermediate)
{
	PNuoMesh resultMesh;
	bool textured = options._textured && !model->GetTexturePathDiffuse().empty();

	if (!textured && !options._basicMaterialized)
	{
		PNuoMeshSimple mesh = std::make_shared<NuoMeshSimple>();
		mesh->Init(commandBuffer, intermediate, std::dynamic_pointer_cast<NuoModelSimple>(model), format, sampleCount);

		resultMesh = mesh;
	}
	/*else if (textured && options._basicMaterialized)
	{
		NSString* modelTexturePath = [NSString stringWithUTF8String : model->GetTexturePathDiffuse().c_str()];
		BOOL embeddedAlpha = options._textureEmbedMaterialTransparency;

		NuoMeshTexMatieraled* mesh = [[NuoMeshTexMatieraled alloc]initWithCommandQueue:commandQueue
			withVerticesBuffer : model->Ptr()
			withLength : model->Length()
			withIndices : model->IndicesPtr()
			withLength : model->IndicesLength()];

		[mesh makeTexture : modelTexturePath checkTransparency : embeddedAlpha] ;

		NSString* modelTexturePathOpacity = [NSString stringWithUTF8String : model->GetTexturePathOpacity().c_str()];
		if ([modelTexturePathOpacity isEqualToString : @""])
			modelTexturePathOpacity = nil;
		if (modelTexturePathOpacity)
			[mesh makeTextureOpacity : modelTexturePathOpacity withCommandQueue : commandQueue];

		NSString* modelTexturePathBump = [NSString stringWithUTF8String : model->GetTexturePathBump().c_str()];
		if ([modelTexturePathBump isEqualToString : @""])
			modelTexturePathBump = nil;
		if (modelTexturePathBump)
			[mesh makeTextureBump : modelTexturePathBump withCommandQueue : commandQueue];

		if (model->HasTransparent() || modelTexturePathOpacity)
			[mesh setTransparency : YES];
		else if (!embeddedAlpha)
			[mesh setTransparency : NO];

		[mesh setIgnoreTexutreAlpha : !embeddedAlpha] ;
		[mesh setPhysicallyReflection : options._physicallyReflection] ;

		resultMesh = mesh;
	}*/
	else if (!textured && options._basicMaterialized)
	{
		PNuoMeshMaterialed mesh = std::make_shared<NuoMeshMaterialed>();
		mesh->SetTransparency(model->HasTransparent());
		mesh->SetPhysicallyReflection(options._physicallyReflection);

		mesh->Init(commandBuffer, intermediate,
				   std::dynamic_pointer_cast<NuoModelMaterialed>(model),
				   format, sampleCount);

		resultMesh = mesh;
	}

	return resultMesh;
}

