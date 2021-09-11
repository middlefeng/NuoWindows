#pragma once

#include <memory>
#include <set>
#include <string>
#include <functional>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoDirect/NuoVertexBuffer.h"
#include "NuoDirect/NuoCommandBuffer.h"
#include "NuoDirect/NuoCommandQueue.h"
#include "NuoUtilites/NuoMathVector.h"

#include <DirectXMath.h>

#include "NuoMeshes/NuoMeshBounds.h"
#include "NuoMeshes/NuoMeshRotation.h"
#include "NuoMeshes/NuoShaders/NuoMeshShaderType.h"
#include "NuoMeshes/NuoShaders/NuoUniforms.h"
#include "NuoMeshes/NuoShaders/NuoMeshSimple.h"



class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;

class NuoMesh;
typedef std::shared_ptr<NuoMesh> PNuoMesh;


class NuoMesh
{

protected:

	NuoMeshRotation _rotation;

	// mesh transform about the poise (rotation around model center).
	// note this transform may include a slight translation before rotation
	// because the model need to be centered (usually around its bounding box)
	//
	NuoMatrixFloat44 _transformPoise;

	// mesh transform about the translate in the world coordinate
	//
	NuoMatrixFloat44 _transformTranslate;

	NuoMeshBounds _boundsLocal;


	// GPU related data structures
	//

	DXGI_FORMAT _format;
	unsigned int _sampleCount;
	PNuoPipelineState _pipelineState;

	PNuoVertexBuffer _vertexBuffer;
	PNuoResourceSwapChain _transformBuffers;

	PNuoPipelineState MakePipelineState(const PNuoCommandBuffer& commandBuffer,
										const std::string& vertex, const std::string& pixel);

	virtual std::vector<D3D12_INPUT_ELEMENT_DESC> InputDesc() = 0;
	virtual PNuoRootSignature RootSignature(const PNuoCommandBuffer& commandBuffer);
	virtual DXGI_FORMAT PipelineFormat();
	virtual PNuoPipelineState PipelineState();
	virtual bool EnableDepth();

public:

	void Init(const PNuoCommandBuffer& commandBuffer, unsigned int frameCount);

	NuoMeshBounds BoundsLocal() const;
	void SetBoundsLocal(const NuoMeshBounds& bounds);
	
	NuoMeshBounds WorldBounds(const NuoMatrixFloat44& transform);

	virtual bool HasTransparency() const = 0;
	virtual void SetTransparency(bool transparency) = 0;

	virtual unsigned int SampleCount();
	virtual void SetSampleCount(unsigned int sampleCount);
	virtual void MakePipelineState(const PNuoCommandBuffer& commandBuffer) = 0;

	virtual void UpdateUniform(unsigned int inFlight, const NuoMatrixFloat44& transform);

	typedef std::function<void(NuoCommandEncoder* encoder)> CommonFunc;

	virtual void DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc func);
	virtual void Draw(const PNuoCommandEncoder& encoder);

	const NuoMatrixFloat44& TransformPoise();
	void SetTransformPoise(const NuoMatrixFloat44& poise);

	void CenterMesh();

protected:

	NuoMatrixFloat44 MeshTransform() const;

};


template <class MeshVertex>
class NuoMeshBase : public NuoMesh
{

public:

	NuoMeshBase() = default;
	virtual ~NuoMeshBase();
	
	void Init(const PNuoCommandBuffer& commandBuffer,
			  unsigned int frameCount,
			  std::vector<PNuoResource>& intermediate,
			  MeshVertex* buffer, size_t number,
			  UINT32* indiciesBuffer, size_t indiciesCount);

};


template <class MeshVertex>
NuoMeshBase<MeshVertex>::~NuoMeshBase()
{
}


template <class MeshVertex>
void NuoMeshBase<MeshVertex>::Init(const PNuoCommandBuffer& commandBuffer,
								   unsigned int frameCount,
		 						   std::vector<PNuoResource>& intermediate,
								   MeshVertex* buffer, size_t number,
								   UINT32* indiciesBuffer, size_t indiciesCount)
{
	_vertexBuffer = std::make_shared<NuoVertexBuffer>(commandBuffer, intermediate,
													  buffer, number * sizeof(MeshVertex), sizeof(MeshVertex),
													  indiciesBuffer, indiciesCount);

	NuoMesh::Init(commandBuffer, frameCount);
}


class NuoModelSimple;
typedef std::shared_ptr<NuoModelSimple> PNuoModelSimple;


class NuoMeshSimple : public NuoMeshBase<NuoMeshSimpleItem>
{

public:

	void Init(const PNuoCommandBuffer& commandBuffer,
			  unsigned int frameCount,
			  std::vector<PNuoResource>& intermediate,
			  const PNuoModelSimple& model,
		      DXGI_FORMAT format);

	virtual bool HasTransparency() const override;
	virtual void SetTransparency(bool transparency) override;

	virtual std::vector<D3D12_INPUT_ELEMENT_DESC> InputDesc() override;
	virtual PNuoRootSignature RootSignature(const PNuoCommandBuffer& commandBuffer) override;
	virtual void MakePipelineState(const PNuoCommandBuffer& commandBuffer) override;

};


typedef std::shared_ptr<NuoMeshSimple> PNuoMeshSimple;


class NuoMeshOptions;
class NuoModelBase;
typedef std::shared_ptr<NuoModelBase> PNuoModelBase;


PNuoMesh CreateMesh(const NuoMeshOptions& options,
					const PNuoCommandBuffer& commandBuffer,
					const PNuoModelBase& model,
				    DXGI_FORMAT format,
					std::vector<PNuoResource>& intermediate);
