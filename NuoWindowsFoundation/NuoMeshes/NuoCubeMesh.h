#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>

#include "NuoMeshes/NuoMesh.h"
#include "NuoUtilites/NuoModelBoard.h"



class NuoCubeMesh;
typedef std::shared_ptr<NuoCubeMesh> PNuoCubeMesh;


class NuoCubeMesh : public NuoMeshBase<NuoItemSimple>
{

	PNuoPipelineState _pipelineState;
	PNuoResourceSwapChain _light;

protected:

	virtual std::vector<D3D12_INPUT_ELEMENT_DESC> InputDesc() override;
	PNuoRootSignature RootSignature(const PNuoCommandBuffer& commandBuffer) override;
	DXGI_FORMAT PipelineFormat() override;

public:

	NuoCubeMesh() = default;

	void Init(const PNuoCommandBuffer& commandBuffer,
			  std::vector<PNuoResource>& intermediate,
			  float width, float height, float depth);

	virtual void Draw(const PNuoCommandEncoder& encoder) override;
	virtual PNuoPipelineState PipelineState() override;

};


