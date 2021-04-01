//
//  NuoRendererPass.h
//  ModelViewer
//
//  Created by Dong Feng on 3/29/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#include <memory>
#include "NuoDirect/NuoSize.h"


class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;

class NuoRenderTarget;
typedef std::shared_ptr<NuoRenderTarget> PNuoRenderTarget;


class NuoRenderPass
{

	PNuoRenderTarget _renderTarget;
	NuoSize _drawableSize;

public:

	virtual void SetDrawableSize(const NuoSize& size);
	virtual void DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) = 0;

};


