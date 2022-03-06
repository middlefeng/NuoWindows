//
//  NuoRenderPassTarget.h
//  ModelViewer
//
//  Created by Dong Feng on 4/1/2021.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#ifndef __NUO_RENDER_PASS_TARGET__
#define __NUO_RENDER_PASS_TARGET__

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoRenderTarget.h"



class NuoRenderPassTarget;
typedef std::shared_ptr<NuoRenderPassTarget> PNuoRenderPassTarget;

class NuoRenderPassAttachment;
typedef std::shared_ptr<NuoRenderPassAttachment> PNuoRenderPassAttachment;

class NuoCommandEncoder;
typedef std::shared_ptr<NuoCommandEncoder> PNuoCommandEncoder;


class NuoRenderPassAttachment
{
	PNuoDevice _device;
	DXGI_FORMAT _format;
	NuoRenderPassTarget* _renderTarget;

	PNuoTexture _resource;
	PNuoTexture _sampleResource;

public:

	NuoRenderPassAttachment(const PNuoDevice& device,
		                    DXGI_FORMAT format, NuoRenderPassTarget* renderTarget);

	void MakeTexture();
	PNuoResource RenderBuffer();
	PNuoTexture Texture();

	void ResourceUseBegin(const PNuoCommandEncoder& encoder);
	void ResourceUseEnd(const PNuoCommandEncoder& encoder);

};



class NuoRenderPassTarget : public NuoRenderTarget
{

	std::vector<PNuoRenderPassAttachment> _attachments;
	PNuoDevice _device;

	std::vector< D3D12_CPU_DESCRIPTOR_HANDLE> _views;

public:

	NuoRenderPassTarget(const PNuoDevice& device,
						unsigned int sampleCount, bool depthEnabled);

	void AddAttachment(DXGI_FORMAT format);
	void Init();

	virtual unsigned int AttachmentNumber() const override;
	virtual D3D12_CPU_DESCRIPTOR_HANDLE* View() override;

	virtual PNuoCommandEncoder RetainRenderPassEncoder(const PNuoCommandBuffer& commandBuffer) override;
	virtual void ReleaseRenderPassEncoder() override;
	virtual PNuoTexture ResultTexture(unsigned int index) const override;

	virtual void SetDrawableSize(const NuoSize& size) override;

};


#endif

