//
//  NuoRenderPipelinePass.h
//  ModelViewer
//
//  Created by Dong Feng on 3/30/21.
//  Copyright © 2021 Dong Feng. All rights reserved.
//


#include "NuoRenderPass.h"


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



};