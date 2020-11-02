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


class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;


class NuoMesh
{
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
			  PNuoResource& intermediate,
			  MeshVertex* buffer, size_t number);

};


template <class MeshVertex>
void NuoMeshBase<MeshVertex>::Init(const PNuoCommandBuffer& commandBuffer,
		 						   PNuoResource& intermediate,
								   MeshVertex* buffer, size_t number)
{
	_vertexBuffer = std::make_shared<NuoVertexBuffer>(commandBuffer, intermediate,
													  buffer, number * sizeof(MeshVertex), sizeof(MeshVertex));
}
