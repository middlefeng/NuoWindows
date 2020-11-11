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
	DirectX::XMFLOAT4 _normal;
	DirectX::XMFLOAT4 _color;
};


class NuoCubeMesh;
typedef std::shared_ptr<NuoCubeMesh> PNuoCubeMesh;


class NuoCubeMesh : public NuoMeshBase<NuoCubeMeshVertex>
{

	PNuoPipelineState _pipelineState;
	PNuoResourceSwapChain _light;

public:

	NuoCubeMesh() = default;

	void Init(const PNuoCommandBuffer& commandBuffer,
			  std::vector<PNuoResource>& intermediate,
			  float width, float height, float depth);

	virtual void Draw(const PNuoCommandEncoder& encoder) override;
	virtual PNuoPipelineState PipelineState() override;

};


