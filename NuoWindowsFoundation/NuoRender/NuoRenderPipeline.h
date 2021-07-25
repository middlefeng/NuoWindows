//
//  NuoRenderPipeline.h
//  ModelViewer
//
//  Created by Dong Feng on 5/21/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#include <memory>
#include <vector>

#include "NuoDirect/NuoSize.h"



class NuoRenderPass;
typedef std::shared_ptr<NuoRenderPass> PNuoRenderPass;

class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;




class NuoRenderPipeline
{

	/**
	 *  The passes of the view's rendering, responsible for maintain the model/scene state,
	 *  and the rendering.
	 *
	 *  Note that the pipeline does the sequential rendering for all the passes but does not
	 *  own the passes (i.e. weak). The passes are constructed and owned by application
	 */
	std::vector<PNuoRenderPass> _renderPasses;


public:

	void RenderWithCommandBuffer(const PNuoCommandBuffer& commandBuffer);

	void SetRenderPasses(const std::vector<PNuoRenderPass>& passes);
	void SetDrawableSize(const NuoSize& size);
	//void SetSampleCount(unsigned int sampleCount);


};