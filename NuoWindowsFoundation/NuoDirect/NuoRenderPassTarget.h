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



class NuoRenderPassTarget : public NuoRenderTarget
{

public:

	NuoRenderPassTarget(const PNuoDevice& device,
						unsigned int width, unsigned int height,
						unsigned int sampleCount, bool depthEnabled);

	void AddAttachment(DXGI_FORMAT format);
	void Init();

};


#endif

