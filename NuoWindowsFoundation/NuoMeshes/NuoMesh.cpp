

#include "NuoMesh.h"

#include "NuoFile.h"
#include "NuoStrings.h"
#include "NuoAppInstance.h"

#include "NuoDirect/NuoResourceSwapChain.h"
#include "NuoMeshes/NuoShaders/NuoUniforms.h"

#include "NuoModelLoader/NuoModelBase.h"
#include "NuoMeshMaterialed.h"




void NuoMesh::Init(const PNuoCommandBuffer& commandBuffer, unsigned int frameCount)
{
	_transformBuffers = std::make_shared<NuoResourceSwapChain>(commandBuffer->CommandQueue()->Device(),
															   frameCount, (unsigned long)sizeof(NuoMeshUniforms));
}


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

	const bool depthEnable = EnableDepth();
	const bool depthWrite = !HasTransparency();

	return std::make_shared<NuoPipelineState>(device, PipelineFormat(), depthEnable, depthWrite, SampleCount(), blendingMode,
											  inputElementDescs, vertexShader, pixelShader, rootSignature);
}


PNuoRootSignature NuoMesh::RootSignature(const PNuoCommandBuffer& commandBuffer)
{
	PNuoDevice device = commandBuffer->CommandQueue()->Device();

	PNuoRootSignature rootSignature = std::make_shared<NuoRootSignature>(device,
																		 D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// parameters defined in NuoUniforms
	//

	rootSignature->AddRootConstantBuffer(0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	rootSignature->AddRootConstantBuffer(1, 0, D3D12_SHADER_VISIBILITY_PIXEL);	// light
	rootSignature->AddRootConstantBuffer(2, 0, D3D12_SHADER_VISIBILITY_VERTEX);	// mesh transform

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


void NuoMesh::SetSampleCount(unsigned int sampleCount)
{
	_sampleCount = sampleCount;
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


NuoMatrixFloat44 NuoMesh::MeshTransform() const
{
	return _rotation.RotationMatrix() * _transformTranslate * _transformPoise;
}


NuoMeshBounds NuoMesh::WorldBounds(const NuoMatrixFloat44& transform)
{
	const NuoBounds& boundsLocal = _boundsLocal.boundingBox;
	const NuoSphere& sphereLocal = _boundsLocal.boundingSphere;

	const NuoMatrixFloat44 transformWorld = transform * MeshTransform();

	NuoMeshBounds worldMeshBounds =
	{
		boundsLocal.Transform(transformWorld),
		sphereLocal.Transform(transformWorld)
	};

	return worldMeshBounds;
}


void NuoMesh::UpdateUniform(unsigned int inFlight, const NuoMatrixFloat44& transform)
{
	NuoMatrixFloat44 transformWorld = transform * MeshTransform();

	NuoMeshUniforms uniforms;
	uniforms.transform = transformWorld._m;
	uniforms.normalTransform = NuoMatrixExtractLinear4(transformWorld)._m;

	_transformBuffers->UpdateResource(&uniforms, sizeof(uniforms), inFlight);
}


void NuoMesh::Draw(const PNuoCommandEncoder& encoder)
{
	encoder->SetRootConstantBuffer(2, _transformBuffers);
	encoder->SetVertexBuffer(_vertexBuffer);
	encoder->DrawIndexed(_vertexBuffer->IndiciesCount());
}


void NuoMesh::DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc func)
{
	encoder->SetPipeline(PipelineState());
	func(encoder.get());
}


const NuoMatrixFloat44& NuoMesh::TransformPoise()
{
	return _transformPoise;
}


void NuoMesh::SetTransformPoise(const NuoMatrixFloat44& poise)
{
	_transformPoise = poise;
}


void NuoMeshSimple::Init(const PNuoCommandBuffer& commandBuffer,
						 unsigned int frameCount,
						 std::vector<PNuoResource>& intermediate,
						 const PNuoModelSimple& model,
						 DXGI_FORMAT format)
{
	_format = format;

	NuoMeshBase<NuoMeshSimpleItem>::Init(commandBuffer, frameCount, intermediate,
										 (NuoMeshSimpleItem*)model->Ptr(),
										 model->GetVerticesNumber(),
										 model->IndicesPtr(),
										 model->IndicesCount());
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



void NuoMeshSimple::MakePipelineState(const PNuoCommandBuffer& commandBuffer)
{
	_pipelineState = NuoMesh::MakePipelineState(commandBuffer, "NuoMeshSimpleVertex", "NuoMeshSimplePixel");
}



PNuoMesh CreateMesh(const NuoMeshOptions& options,
					const PNuoCommandBuffer& commandBuffer,
					const PNuoModelBase& model,
					DXGI_FORMAT format,
					std::vector<PNuoResource>& intermediate)
{
	PNuoMesh resultMesh;
	bool textured = options._textured && !model->GetTexturePathDiffuse().empty();

	if (!textured && !options._basicMaterialized)
	{
		PNuoMeshSimple mesh = std::make_shared<NuoMeshSimple>();
		mesh->Init(commandBuffer, options._frameCount, intermediate, std::dynamic_pointer_cast<NuoModelSimple>(model), format);

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

		mesh->Init(commandBuffer, options._frameCount, intermediate,
				   std::dynamic_pointer_cast<NuoModelMaterialed>(model),
				   format);

		resultMesh = mesh;
	}

	resultMesh->SetSampleCount(options._sampleCount);
	resultMesh->MakePipelineState(commandBuffer);

	return resultMesh;
}

