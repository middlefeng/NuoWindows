﻿

#include "NuoSignature.h"


#include <cassert>


NuoRootSignature::NuoRootSignature(const PNuoDevice& device,
								   D3D12_ROOT_SIGNATURE_FLAGS flags)
	: _device(device)
{
	D3D12_ROOT_SIGNATURE_DESC1 desc = {};
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


void NuoRootSignature::AddConstant(size_t size, unsigned int shaderRegister, unsigned int space, 
								   D3D12_SHADER_VISIBILITY visibility)
{
	D3D12_ROOT_PARAMETER1 param;
	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	param.ShaderVisibility = visibility;
	param.Constants.Num32BitValues = (UINT)(size / 4);
	param.Constants.ShaderRegister = (UINT)shaderRegister;
	param.Constants.RegisterSpace = (UINT)space;

	_parameters.push_back(param);

	UpdateDesc();
}


void NuoRootSignature::AddRootConstantBuffer(unsigned int shaderRegister, unsigned int space,
											 D3D12_SHADER_VISIBILITY visibility)
{
	AddConstantView(shaderRegister, space, visibility, D3D12_ROOT_PARAMETER_TYPE_CBV);
}


void NuoRootSignature::AddRootResource(unsigned int shaderRegister, unsigned int space, D3D12_SHADER_VISIBILITY visibility)
{
	AddConstantView(shaderRegister, space, visibility, D3D12_ROOT_PARAMETER_TYPE_SRV);
}


void NuoRootSignature::AddSampler(unsigned int shaderRegister, unsigned int space, D3D12_SHADER_VISIBILITY visibility)
{
	D3D12_STATIC_SAMPLER_DESC desc = {};

	desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 0;
	desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	desc.MinLOD = 0.0f;
	desc.MaxLOD = D3D12_FLOAT32_MAX;
	desc.ShaderRegister = shaderRegister;
	desc.RegisterSpace = space;
	desc.ShaderVisibility = visibility;

	_samplers.push_back(desc);

	UpdateDesc();
}


int NuoRootSignature::AddDescriptorTable(unsigned int rangeNum, D3D12_SHADER_VISIBILITY visibility)
{
	D3D12_ROOT_PARAMETER1 param = {};

	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param.ShaderVisibility = visibility;
	param.DescriptorTable.NumDescriptorRanges = rangeNum;

	// the descriptor table is next to the last existing parameter of the root signature,
	// this indexOfDescriptorTableInRootSignature is used to track all the table descriptor ranges
	// in the _descriptorTableRanges map
	//
	size_t indexOfDescriptorTableInRootSignature = _parameters.size();
	_descriptorTableRanges.insert(std::make_pair(indexOfDescriptorTableInRootSignature,
												 std::vector<D3D12_DESCRIPTOR_RANGE1>(rangeNum)));

	param.DescriptorTable.pDescriptorRanges = _descriptorTableRanges[indexOfDescriptorTableInRootSignature].data();
	
	// again, this confirms that the table is next to the last existing param
	//
	_parameters.push_back(param);

	UpdateDesc();

	return (int)indexOfDescriptorTableInRootSignature;
}


void NuoRootSignature::AddTexturesToDescriptorTable(unsigned int tableIndex, unsigned int rangeIndex, unsigned int textureNum,
													unsigned int shaderRegister, unsigned int space)
{
	auto ranges = &_descriptorTableRanges.find(tableIndex)->second;

	assert(ranges->begin() + rangeIndex < ranges->end());
	std::vector<D3D12_DESCRIPTOR_RANGE1>::iterator range = (ranges->begin() + rangeIndex);

	range->BaseShaderRegister = shaderRegister;
	range->RegisterSpace = space;
	range->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range->Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
	range->NumDescriptors = textureNum;
	range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}


void NuoRootSignature::UpdateDesc()
{
	_signature.Reset();

	_desc.Desc_1_1.NumParameters = (UINT)_parameters.size();
	_desc.Desc_1_1.pParameters = _parameters.data();
	_desc.Desc_1_1.NumStaticSamplers = (UINT)_samplers.size();
	_desc.Desc_1_1.pStaticSamplers = _samplers.data();
}



void NuoRootSignature::AddConstantView(unsigned int shaderRegister, unsigned int space,
									   D3D12_SHADER_VISIBILITY visibility, D3D12_ROOT_PARAMETER_TYPE type)
{
	D3D12_ROOT_PARAMETER1 param;

	param.ParameterType = type;
	param.ShaderVisibility = visibility;
	param.Descriptor.ShaderRegister = shaderRegister;
	param.Descriptor.RegisterSpace = space;
	param.Descriptor.Flags = D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE;

	_parameters.push_back(param);

	UpdateDesc();
}

