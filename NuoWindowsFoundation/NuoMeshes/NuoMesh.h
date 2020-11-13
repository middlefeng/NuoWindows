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

#include <DirectXMath.h>

#include "NuoMeshes/NuoShaders/NuoMeshShaderType.h"
#include "NuoMeshes/NuoShaders/NuoUniforms.h"
#include "NuoMeshes/NuoShaders/NuoMeshSimpile.h"

#include "NuoMeshes/NuoCubeMesh_V.h"


class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;



class NuoMesh
{

protected:

	PNuoPipelineState MakePipelineState(const PNuoCommandBuffer& commandBuffer,
										const std::string& vertex, const std::string& pixel);

	virtual std::vector<D3D12_INPUT_ELEMENT_DESC> InputDesc() = 0;
	virtual PNuoRootSignature RootSignature(const PNuoCommandBuffer& commandBuffer) = 0;
	virtual DXGI_FORMAT PipelineFormat() = 0;

public:

	typedef std::function<void(NuoCommandEncoder* encoder)> CommonFunc;

	virtual void DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc& func);
	virtual void Draw(const PNuoCommandEncoder& encoder) = 0;

	virtual PNuoPipelineState PipelineState() = 0;
};


template <class MeshVertex>
class NuoMeshBase : public NuoMesh
{

protected:

	PNuoVertexBuffer _vertexBuffer;

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
}


class NuoModelSimple;
typedef std::shared_ptr<NuoModelSimple> PNuoModelSimple;


class NuoMeshSimple : public NuoMeshBase<NuoMeshSimpleItem>
{

public:

	void Init(const PNuoCommandBuffer& commandBuffer, 
			  std::vector<PNuoResource>& intermediate,
			  const PNuoModelSimple& model);

};

