//
//  NuoRenderPipelinePass.h
//  ModelViewer
//
//  Created by Dong Feng on 3/30/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//


#include "NuoRenderPass.h"

#include <memory>
#include <vector>
#include <dxgi1_6.h>


class NuoTextureMesh;
typedef std::shared_ptr<NuoTextureMesh> PNuoTextureMesh;

class NuoTexture;
typedef std::shared_ptr<NuoTexture> PNuoTexture;

class NuoResource;
typedef std::shared_ptr<NuoResource> PNuoResource;


/**
 *  a render pass takes the result from its immediate previous
 *  pass, draw it on the background, and leave its subclass draw method to
 *  add additional objects
 *
 *  also, it can take the source as an texture of a pixel format and draw it
 *  to the target of another format, in that way serving as pixel format convertor.
 *
 *  in the conversion-only case, or in a 2D only case, the sampleCount could be
 *  set to 1 to turning off the MSAA
 */

class NuoRenderPipelinePass : public NuoRenderPass
{

	PNuoTextureMesh _textureMesh;
	

public:

	NuoRenderPipelinePass(const PNuoCommandBuffer& commandBuffer,
						  std::vector<PNuoResource>& intermediate,
						  DXGI_FORMAT format,
						  unsigned int sampleCount);

	void SetSourceTextrue(const PNuoTexture& texture);

	virtual void DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) override;
	virtual bool IsPipelinePass() const override;

};