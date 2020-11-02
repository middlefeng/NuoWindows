

#include "NuoCubeMesh.h"


void NuoCubeMesh::Init(const PNuoCommandBuffer& commandBuffer,
					   PNuoResource& intermediate,
					   float width, float height, float depth)
{
	std::vector<NuoCubeMeshVertex> vertices =
	{
		{ DirectX::XMFLOAT4(-width / 2.0f, -height / 2.0, -depth / 2.0, 1.0) },
		{ DirectX::XMFLOAT4(-width / 2.0f,  height / 2.0, -depth / 2.0, 1.0) },
		{ DirectX::XMFLOAT4( width / 2.0f,  height / 2.0, -depth / 2.0, 1.0) },
		{ DirectX::XMFLOAT4( width / 2.0f, -height / 2.0, -depth / 2.0, 1.0) },
		{ DirectX::XMFLOAT4(-width / 2.0f, -height / 2.0,  depth / 2.0, 1.0) },
		{ DirectX::XMFLOAT4(-width / 2.0f,  height / 2.0,  depth / 2.0, 1.0) },
		{ DirectX::XMFLOAT4( width / 2.0f,  height / 2.0,  depth / 2.0, 1.0) },
		{ DirectX::XMFLOAT4( width / 2.0f, -height / 2.0,  depth / 2.0, 1.0) },
	};

	NuoMeshBase<NuoCubeMeshVertex>::Init(commandBuffer, intermediate,
										 vertices.data(), vertices.size());
}


void NuoCubeMesh::Draw(const PNuoCommandEncoder& encoder)
{

}
