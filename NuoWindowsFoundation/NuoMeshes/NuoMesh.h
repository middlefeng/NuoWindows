#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoDirect/NuoVertexBuffer.h"
#include "NuoDirect/NuoCommandBuffer.h"
#include "NuoDirect/NuoCommandQueue.h"

#include <DirectXMath.h>


class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;


struct NuoModelViewProjection
{
	DirectX::XMMATRIX _mvpMatrix;
	DirectX::XMMATRIX _normalMatrix;
};


class NuoMesh
{
public:

	virtual void Draw(const PNuoCommandEncoder& encoder) = 0;
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
			  UINT* indiciesBuffer, size_t indiciesCount);

};


template <class MeshVertex>
void NuoMeshBase<MeshVertex>::Init(const PNuoCommandBuffer& commandBuffer,
		 						   std::vector<PNuoResource>& intermediate,
								   MeshVertex* buffer, size_t number,
								   UINT* indiciesBuffer, size_t indiciesCount)
{
	_vertexBuffer = std::make_shared<NuoVertexBuffer>(commandBuffer, intermediate,
													  buffer, number * sizeof(MeshVertex), sizeof(MeshVertex),
													  indiciesBuffer, indiciesCount);
}
