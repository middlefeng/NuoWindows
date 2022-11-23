#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;


class NuoDescriptorHeap;
typedef std::shared_ptr<NuoDescriptorHeap> PNuoDescriptorHeap;
typedef std::weak_ptr<NuoDescriptorHeap> WPNuoDescriptorHeap;

class NuoTexture;
typedef std::shared_ptr<NuoTexture> PNuoTexture;

class NuoResource;
typedef std::shared_ptr<NuoResource> PNuoResource;


enum NuoDescriptorType
{
	kNuoDescriptor_RenderTarget,
	kNuoDescriptor_DepthStencil,
	kNuoDescriptor_ShaderResource
};



class NuoDescriptorHeap
{	

	PNuoDevice _device;
	
	NuoDescriptorType _type;
	unsigned int _size;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _heap;

private:

	NuoDescriptorHeap(NuoDescriptorType type);

public:

	unsigned int Incremental();

	D3D12_CPU_DESCRIPTOR_HANDLE DxCPUHandle(unsigned int inFlight);
	D3D12_GPU_DESCRIPTOR_HANDLE DxGPUHandle(unsigned int inFlight);
	D3D12_CPU_DESCRIPTOR_HANDLE DxHeapCPUHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE DxHeapGPUHandle();

	void SetTexture(unsigned int index, const PNuoTexture& texture);
	void SetTextureMipSlice(unsigned int index,
						    const PNuoTexture& texture,
						    unsigned int mipSlice);

	void SetConstantBuffer(unsigned int index, const PNuoResource& buffer);

	ID3D12DescriptorHeap* DxHeap() const;

	friend class NuoDevice;

};

