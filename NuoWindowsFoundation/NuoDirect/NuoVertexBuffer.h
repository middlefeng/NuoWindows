#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoResource.h"

class NuoVertexBuffer;
typedef std::shared_ptr<NuoVertexBuffer> PNuoVertexBuffer;
typedef std::weak_ptr<NuoVertexBuffer> WPNuoVertexBuffer;

class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;
typedef std::weak_ptr<NuoCommandBuffer> WPNuoCommandBuffer;


class NuoVertexBuffer
{
	PNuoResource _buffer;
	PNuoResource _indicies;

	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW _indiciesBufferView;
	
public:

	NuoVertexBuffer(const PNuoCommandBuffer& commandBuffer,
					std::vector<PNuoResource>& intermediatePool,
					void* data, size_t size, size_t stride,
					void* indicies, size_t indiciesCount);

	NuoVertexBuffer(const PNuoResource& buffer, size_t stride);
	D3D12_VERTEX_BUFFER_VIEW* View();
	D3D12_INDEX_BUFFER_VIEW* IndiciesView();

	unsigned int Count() const;
	unsigned int IndiciesCount() const;

private:

	void UpdateView(size_t stride);
};

