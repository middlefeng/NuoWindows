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
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
	
public:

	NuoVertexBuffer(const PNuoCommandBuffer& commandBuffer,
					PNuoResource& intermediate,
					void* data, size_t size, size_t stride);

	NuoVertexBuffer(const PNuoResource& buffer, size_t stride);
	D3D12_VERTEX_BUFFER_VIEW* View();

	unsigned int Count() const;

private:

	void UpdateView(size_t stride);
};

