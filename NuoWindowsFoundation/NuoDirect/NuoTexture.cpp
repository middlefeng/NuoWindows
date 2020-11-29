﻿

#include "NuoTexture.h"

#include "NuoDevice.h"
#include "NuoCommandBuffer.h"

#include <cassert>
#include "d3dx12.h"


NuoTexture::NuoTexture()
{
}


NuoTexture::~NuoTexture()
{
}



void NuoTexture::Upload(const PNuoCommandBuffer& commandBuffer, const std::vector<UINT8>& data,
						std::vector<PNuoResource>& intermediate)
{
	PNuoCommandEncoder encoder = commandBuffer->CreateRenderPassEncoder();
	encoder->CopyTexture(data, intermediate, std::dynamic_pointer_cast<NuoTexture>(shared_from_this()));
	encoder->EndEncoding();
}

