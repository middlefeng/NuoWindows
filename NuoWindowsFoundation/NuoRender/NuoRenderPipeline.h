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


class NuoTexture;
typedef std::shared_ptr<NuoTexture> PNuoTexture;

class NuoRenderPass;
typedef std::shared_ptr<NuoRenderPass> PNuoRenderPass;

class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;


/**
 *  use this delegate to defer the retrival of the final target texture.
 *  the retrival is time consuming in some case and should be deferred until
 *  the very last pass
 */
class NuoRenderPipelineDelegate
{

public:

	virtual PNuoTexture NextFinalTexture() = 0;

};



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

	NuoRenderPipelineDelegate* _renderPipelineDelegate;


public:

	bool RenderWithCommandBuffer(const PNuoCommandBuffer& commandBuffer);

	void SetDrawableSize(const NuoSize& size);
	//void SetSampleCount(unsigned int sampleCount);


};