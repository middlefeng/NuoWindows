#pragma once

#include <memory>
#include <vector>
#include <map>
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
	std::vector<D3D12_STATIC_SAMPLER_DESC> _samplers;

	std::map<size_t, std::vector<D3D12_DESCRIPTOR_RANGE1>> _descriptorTableRanges;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> _signature;
	D3D12_VERSIONED_ROOT_SIGNATURE_DESC _desc;

	PNuoDevice _device;

public:

	NuoRootSignature(const PNuoDevice& device,
					 D3D12_ROOT_SIGNATURE_FLAGS flags);

	void AddConstant(size_t size, unsigned int shaderRegister, unsigned int space, D3D12_SHADER_VISIBILITY visibility);
	void AddRootConstantBuffer(unsigned int shaderRegister, unsigned int space, D3D12_SHADER_VISIBILITY visibility);
	void AddRootResource(unsigned int shaderRegister, unsigned int space, D3D12_SHADER_VISIBILITY visibility);
	void AddSampler(unsigned int shaderRegister, unsigned int space, D3D12_SHADER_VISIBILITY visibility);

	void AddDescriptorTable(unsigned int rangeNum, D3D12_SHADER_VISIBILITY visibility);
	void AddTextures(unsigned int index, unsigned int rangeIndex, unsigned int num,
					 unsigned int shaderRegister, unsigned int space);

	ID3D12RootSignature* DxSignature();

private:

	void UpdateDesc();
	void AddConstantView(unsigned int shaderRegister, unsigned int space,
						 D3D12_SHADER_VISIBILITY visibility, D3D12_ROOT_PARAMETER_TYPE type);

};


typedef std::shared_ptr<NuoRootSignature> PNuoRootSignature;
typedef std::weak_ptr<NuoRootSignature> WPNuoRootSignature;




