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

	virtual std::vector<D3D12_INPUT_ELEMENT_DESC> InputDesc() override;

	virtual void DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc func) override;
	virtual void Draw(const PNuoCommandEncoder& encoder) override;

};

