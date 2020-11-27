

#include "NuoTexture.h"

#include "NuoDevice.h"
#include "NuoCommandBuffer.h"
#include <cassert>


NuoTexture::NuoTexture()
{
}


NuoTexture::~NuoTexture()
{
}



void NuoTexture::Upload(const PNuoCommandBuffer& commandBuffer, const std::vector<UINT8>& data,
						std::vector<PNuoResource> intermediate)
{
	size_t dataSize = Width() * 4 * Height();
	assert(data.size() == dataSize);

	PNuoResource upload = commandBuffer->CommandQueue()->Device()->CreateUploadBuffer(Width() * 4 * Height());
	intermediate.push_back(upload);

	void* dst = upload->Map();
	memcpy(dst, data.data(), dataSize);
	upload->Unmap();

	commandBuffer->CopyResource(upload, std::dynamic_pointer_cast<NuoResource>(shared_from_this()));
}

