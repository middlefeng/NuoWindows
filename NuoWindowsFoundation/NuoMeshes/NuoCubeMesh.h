#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>

#include "NuoMeshes/NuoMesh.h"



struct NuoCubeMeshVertex
{
	DirectX::XMFLOAT4 _position;
};


class NuoCubeMesh : public NuoMeshBase<NuoCubeMeshVertex>
{

	

public:

	NuoCubeMesh() = default;

	void Init(const PNuoCommandBuffer& commandBuffer,
			  PNuoResource& intermediate,
			  float width, float height, float depth);

	virtual void Draw(const PNuoCommandEncoder& encoder) override;

};


