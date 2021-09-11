

#ifndef __MODEL_VIEWER_RENDERER_H__
#define __MODEL_VIEWER_RENDERER_H__

#pragma once

#include "NuoRender/NuoRenderPipelinePass.h"

#include "NuoUtilites/NuoMathVector.h"

#include <dxgi1_6.h>


class ModelState;
typedef std::shared_ptr<ModelState> PModelState;


class NuoResourceSwapChain;
typedef std::shared_ptr<NuoResourceSwapChain> PNuoResourceSwapChain;


class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;



class ModelRenderer : public NuoRenderPipelinePass
{

	PModelState _modelState;

	PNuoTextureMesh _textureMesh;
	PNuoResourceSwapChain _light;
	PNuoResourceSwapChain _mvp;

	PNuoRenderTarget _intermediateTarget;

public:

	ModelRenderer(const PNuoCommandBuffer& commandBuffer, unsigned int frameCount,
				  std::vector<PNuoResource>& intermediate, DXGI_FORMAT format,
				  unsigned int width, unsigned int height);

	PModelState State() const;

	virtual void SetDrawableSize(const NuoSize& size) override;
	virtual void SetSampleCount(unsigned int sampleCount) override;

	virtual void PredrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) override;
	virtual void DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) override;

	void UpdateUniformsForView();

	void Rotate(float dx, float dy);

};


#endif
