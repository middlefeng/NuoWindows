//
//  NuoRendererPass.h
//  ModelViewer
//
//  Created by Dong Feng on 3/29/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#include <memory>


class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;


class NuoRenderPass
{

public:

	virtual void DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) = 0;

};


