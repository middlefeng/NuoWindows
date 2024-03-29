#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>


#include "NuoDevice.h"
#include "NuoShader.h"
#include "NuoSignature.h"

class NuoPipelineState;
typedef std::shared_ptr<NuoPipelineState> PNuoPipelineState;
typedef std::weak_ptr<NuoPipelineState> WPNuoPipelineState;



enum NuoBlendingMode
{
	kNuoBlending_None,
	kNuoBlending_Alpha,
	kNuoBlending_Accumulate,
};


enum NuoCullMode
{
	kNuoCull_None,
	kNuoCull_Front,
	kNuoCull_Back,
};



class NuoPipelineState
{

	Microsoft::WRL::ComPtr<ID3D12PipelineState> _dxPipelineState;
	PNuoRootSignature _rootSignature;


public:

	NuoPipelineState(const PNuoDevice& device,
					 DXGI_FORMAT format,
					 bool depthEnabled, bool depthWrite,
					 unsigned int sampleCount,
					 NuoBlendingMode blendingMode,
					 NuoCullMode cullMode,
					 const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputDesc,
				  	 const PNuoShader& vertex,
					 const PNuoShader& pixel,
					 const PNuoRootSignature& rootSignature);


	ID3D12PipelineState* DxPipeline() const;
	ID3D12RootSignature* DxRootSignature() const;
	
};

