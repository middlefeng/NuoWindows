#pragma once

#include <memory>
#include <vector>


#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoMesh.h"



class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;



class NuoMeshCompound : public NuoMesh
{

protected:

	std::vector<PNuoMesh> _meshes;

	CommonFunc _commonFunc;
	
public:

	NuoMeshCompound(const std::vector<PNuoMesh>& meshes);

	void SetMeshes(const std::vector<PNuoMesh>& meshes);

	virtual NuoMeshBounds WorldBounds(const NuoMatrixFloat44& transform) override;

	virtual std::vector<D3D12_INPUT_ELEMENT_DESC> InputDesc() override;
	virtual void MakePipelineState(const PNuoCommandBuffer& commandBuffer) override;

	virtual bool HasTransparency() const override;
	virtual void SetTransparency(bool transparency) override;

	virtual void UpdateUniform(unsigned int inFlight, const NuoMatrixFloat44& transform) override;
	virtual void DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc func) override;
	virtual void Draw(const PNuoCommandEncoder& encoder) override;

};

