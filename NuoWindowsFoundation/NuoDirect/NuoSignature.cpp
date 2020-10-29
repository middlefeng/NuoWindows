

#include "NuoSignature.h"


#include <cassert>


NuoRootSignature::NuoRootSignature(const PNuoDevice& device,
								   const std::vector<D3D12_ROOT_PARAMETER1>& parameters,
								   const std::vector< D3D12_STATIC_SAMPLER_DESC>& samplers,
								   D3D12_ROOT_SIGNATURE_FLAGS flags)
	: _parameters(parameters),
	  _samplers(samplers)
{
	D3D12_ROOT_SIGNATURE_DESC1 desc;
	desc.NumParameters = parameters.size();
	desc.pParameters = parameters.size() ? _parameters.data() : nullptr;
	desc.NumStaticSamplers = samplers.size();
	desc.pStaticSamplers = samplers.size() ? _samplers.data() : nullptr;

	desc.Flags = flags;

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC descVersion;
	descVersion.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
	descVersion.Desc_1_1 = desc;
	
	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	Microsoft::WRL::ComPtr<ID3DBlob> error;
	D3D12SerializeVersionedRootSignature(&descVersion, &signature, &error);
	
	HRESULT hr = device->DxDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
														 IID_PPV_ARGS(&_signature));

	assert(hr == S_OK);
}


ID3D12RootSignature* NuoRootSignature::DxSignature()
{
	return _signature.Get();
}


