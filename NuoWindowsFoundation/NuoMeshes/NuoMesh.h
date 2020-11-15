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

#include "NuoMeshes/NuoShaders/NuoMeshShaderType.h"
#include "NuoMeshes/NuoShaders/NuoUniforms.h"
#include "NuoMeshes/NuoShaders/NuoMeshSimple.h"

#include "NuoMeshes/NuoCubeMesh_V.h"


class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;

class NuoMesh;
typedef std::shared_ptr<NuoMesh> PNuoMesh;


class NuoMesh
{

protected:

	PNuoVertexBuffer _vertexBuffer;
	PNuoResourceSwapChain _transform;

	PNuoPipelineState MakePipelineState(const PNuoCommandBuffer& commandBuffer,
										const std::string& vertex, const std::string& pixel);

	virtual std::vector<D3D12_INPUT_ELEMENT_DESC> InputDesc() = 0;
	virtual PNuoRootSignature RootSignature(const PNuoCommandBuffer& commandBuffer);
	virtual DXGI_FORMAT PipelineFormat() = 0;

public:

	//virtual void UpdateUniforms(const NuoMatrixFloat44& world);

	typedef std::function<void(NuoCommandEncoder* encoder)> CommonFunc;

	virtual void DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc& func);
	virtual void Draw(const PNuoCommandEncoder& encoder);

	virtual PNuoPipelineState PipelineState() = 0;
};


template <class MeshVertex>
class NuoMeshBase : public NuoMesh
{

public:

	NuoMeshBase() = default;
	
	void Init(const PNuoCommandBuffer& commandBuffer,
			  std::vector<PNuoResource>& intermediate,
			  MeshVertex* buffer, size_t number,
			  UINT32* indiciesBuffer, size_t indiciesCount);

};


template <class MeshVertex>
void NuoMeshBase<MeshVertex>::Init(const PNuoCommandBuffer& commandBuffer,
		 						   std::vector<PNuoResource>& intermediate,
								   MeshVertex* buffer, size_t number,
								   UINT32* indiciesBuffer, size_t indiciesCount)
{
	_vertexBuffer = std::make_shared<NuoVertexBuffer>(commandBuffer, intermediate,
													  buffer, number * sizeof(MeshVertex), sizeof(MeshVertex),
													  indiciesBuffer, indiciesCount);

	//_transform = std::make_shared<NuoResourceSwapChain>(commandBuffer->CommandQueue()->Device(),
	//													commandBuffer->FrameCount(), sizeof(NuoLightVertexUniforms));
}


class NuoModelSimple;
typedef std::shared_ptr<NuoModelSimple> PNuoModelSimple;


class NuoMeshSimple : public NuoMeshBase<NuoMeshSimpleItem>
{

private:

	DXGI_FORMAT _format;
	PNuoPipelineState _pipelineState;

public:

	void Init(const PNuoCommandBuffer& commandBuffer, 
			  std::vector<PNuoResource>& intermediate,
			  const PNuoModelSimple& model,
		      DXGI_FORMAT format);

	virtual std::vector<D3D12_INPUT_ELEMENT_DESC> InputDesc() override;
	virtual PNuoRootSignature RootSignature(const PNuoCommandBuffer& commandBuffer) override;
	virtual DXGI_FORMAT PipelineFormat() override;

	virtual PNuoPipelineState PipelineState();

};

