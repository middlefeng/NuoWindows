

#ifndef __MODEL_VIEWER_RENDERER_H__
#define __MODEL_VIEWER_RENDERER_H__

#pragma once

#include "NuoRender/NuoRenderPipelinePass.h"

#include "NuoUtilites/NuoMathVector.h"


class ModelState;
typedef std::shared_ptr<ModelState> PModelState;


class NuoResourceSwapChain;
typedef std::shared_ptr<NuoResourceSwapChain> PNuoResourceSwapChain;




class ModelRenderer : public NuoRenderPipelinePass
{

	PModelState _modelState;

	PNuoTextureMesh _textureMesh;
	PNuoResourceSwapChain _light;
	PNuoResourceSwapChain _mvp;

	PNuoRenderTarget _intermediateTarget;

	NuoMatrixFloat44 _modelTransfer;

public:

	virtual void DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) override;

};


#endif
