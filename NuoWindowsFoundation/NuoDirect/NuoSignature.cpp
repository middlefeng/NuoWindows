

#include "NuoSignature.h"


#include <cassert>


NuoRootSignature::NuoRootSignature(const PNuoDevice& device,
								   D3D12_ROOT_SIGNATURE_FLAGS flags)
	: _device(device)
{
	D3D12_ROOT_SIGNATURE_DESC1 desc;
	desc.NumParameters = 0;
	desc.pParameters = nullptr;
	desc.NumStaticSamplers = 0;
	desc.pStaticSamplers = nullptr;

	desc.Flags = flags;

	_desc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
	_desc.Desc_1_1 = desc;
}


ID3D12RootSignature* NuoRootSignature::DxSignature()
{
	if (!_signature)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> signature;
		Microsoft::WRL::ComPtr<ID3DBlob> error;
		D3D12SerializeVersionedRootSignature(&_desc, &signature, &error);

		HRESULT hr = _device->DxDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
															  IID_PPV_ARGS(&_signature));

		assert(hr == S_OK);
	}

	return _signature.Get();
}


