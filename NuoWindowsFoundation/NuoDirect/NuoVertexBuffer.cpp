

#include "NuoVertexBuffer.h"

#include "NuoCommandBuffer.h"
#include "NuoCommandQueue.h"


NuoVertexBuffer::NuoVertexBuffer(const PNuoCommandBuffer& commandBuffer,
								 std::vector<PNuoResource>& intermediatePool,
								 void* data, size_t size, size_t stride,
								 void* indicies, size_t indiciesCount)
{
	PNuoDevice device = commandBuffer->CommandQueue()->Device();

	auto intermediate = device->CreateBuffer(data, size);
	_buffer = device->CreateBuffer(size);
	intermediatePool.push_back(intermediate);
	commandBuffer->CopyResource(intermediate, _buffer);

	const size_t indiciesBufferSize = indiciesCount * sizeof(UINT32);
	intermediate = device->CreateBuffer(indicies, indiciesBufferSize);
	_indicies = device->CreateBuffer(indiciesBufferSize);
	intermediatePool.push_back(intermediate);
	commandBuffer->CopyResource(intermediate, _indicies);

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

	_indiciesBufferView.BufferLocation = _indicies->DxResource()->GetGPUVirtualAddress();
	_indiciesBufferView.Format = DXGI_FORMAT_R32_UINT;
	_indiciesBufferView.SizeInBytes = _indicies->Size();
}


D3D12_VERTEX_BUFFER_VIEW* NuoVertexBuffer::View()
{
	return &_vertexBufferView;
}


D3D12_INDEX_BUFFER_VIEW* NuoVertexBuffer::IndiciesView()
{
	return &_indiciesBufferView;
}


unsigned int NuoVertexBuffer::Count() const
{
	return _vertexBufferView.SizeInBytes / _vertexBufferView.StrideInBytes;
}


unsigned int NuoVertexBuffer::IndiciesCount() const
{
	return _indiciesBufferView.SizeInBytes / sizeof(UINT32);
}


