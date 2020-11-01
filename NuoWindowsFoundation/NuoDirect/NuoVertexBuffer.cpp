

#include "NuoVertexBuffer.h"



NuoVertexBuffer::NuoVertexBuffer(const PNuoResource& buffer, size_t stride)
	: _buffer(buffer)
{
	_vertexBufferView.BufferLocation = buffer->DxResource()->GetGPUVirtualAddress();
	_vertexBufferView.SizeInBytes = buffer->Size();
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


