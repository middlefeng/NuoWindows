#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>


#include "NuoDevice.h"
#include "NuoShader.h"




class NuoRootSignature
{
	D3D12_ROOT_SIGNATURE_DESC1 _dxDesc;
	std::vector<D3D12_ROOT_PARAMETER1> _parameters;
	std::vector< D3D12_STATIC_SAMPLER_DESC> _samplers;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> _signature;


public:

	NuoRootSignature(const PNuoDevice& device,
					 const std::vector<D3D12_ROOT_PARAMETER1>& parameters,
					 const std::vector< D3D12_STATIC_SAMPLER_DESC>& samplers,
					 D3D12_ROOT_SIGNATURE_FLAGS flags);

	ID3D12RootSignature* DxSignature();
};


typedef std::shared_ptr<NuoRootSignature> PNuoRootSignature;
typedef std::weak_ptr<NuoRootSignature> WPNuoRootSignature;




