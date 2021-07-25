

#ifndef __MODEL_VIEWER_RENDERER_H__
#define __MODEL_VIEWER_RENDERER_H__

#pragma once

#include "NuoRender/NuoRenderPipelinePass.h"


class ModelRenderer : public NuoRenderPipelinePass
{

public:

	virtual void DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) override;

};


#endif
