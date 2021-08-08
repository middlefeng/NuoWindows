#pragma once

#include <memory>
#include <set>
#include <string>
#include <functional>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>


#include "NuoMesh.h"
#include "NuoShaders/NuoMeshMaterialed.h"
#include "NuoModelLoader/NuoModelMaterialedBasic.h"


typedef std::shared_ptr<NuoModelMaterialed> PNuoModelMaterialed;


class NuoMeshMaterialed : public NuoMeshBase<NuoMaterialedBasicItem>
{

	bool _hasTransparency;
	bool _physicallyReflection;

protected:

	virtual std::vector<D3D12_INPUT_ELEMENT_DESC> InputDesc() override;

public:

	NuoMeshMaterialed();
	virtual ~NuoMeshMaterialed();

	void Init(const PNuoCommandBuffer& commandBuffer,
			  unsigned int frameCount,
			  std::vector<PNuoResource>& intermediate,
			  const PNuoModelMaterialed& model,
			  DXGI_FORMAT format);

	virtual bool HasTransparency() const override;
	virtual void SetTransparency(bool transparency) override;
	virtual void SetPhysicallyReflection(bool physically);

	virtual void MakePipelineState(const PNuoCommandBuffer& commandBuffer) override;

};


typedef std::shared_ptr<NuoMeshMaterialed> PNuoMeshMaterialed;
