

#include "NuoVertexBuffer.h"

#include "NuoCommandBuffer.h"
#include "NuoCommandQueue.h"


NuoVertexBuffer::NuoVertexBuffer(const PNuoCommandBuffer& commandBuffer,
								 PNuoResource& intermediate,
							     void* data, size_t size, size_t stride)
{
	PNuoDevice device = commandBuffer->CommandQueue()->Device();

	intermediate = device->CreateBuffer(data, size);
	_buffer = device->CreateBuffer(size);

	commandBuffer->CopyResource(intermediate, _buffer);
	UpdateView(stride);
}


NuoVertexBuffer::NuoVertexBuffer(const PNuoResource& buffer, size_t stride)
	: _buffer(buffer)
{
	UpdateView(stride);
}


void NuoVertexBuffer::UpdateView(size_t stride)
{
	_vertexBufferView.BufferLocation = _buffer->DxResource()->GetGPUVirtualAddress();
	_vertexBufferView.SizeInBytes = _buffer->Size();
	_vertexBufferView.StrideInBytes = stride;
}


D3D12_VERTEX_BUFFER_VIEW* NuoVertexBuffer::View()
{
	return &_vertexBufferView;
}


unsigned int NuoVertexBuffer::Count() const
{
	return _vertexBufferView.SizeInBytes / _vertexBufferView.StrideInBytes;
}


