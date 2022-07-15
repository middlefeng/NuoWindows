

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

class ModelSceneParameters;
typedef std::shared_ptr<ModelSceneParameters> PModelSceneParameters;



class ModelRenderer : public NuoRenderPipelinePass
{

	PModelState _modelState;

	PModelSceneParameters _sceneParameters;

	PNuoTextureMesh _textureMesh;
	PNuoResourceSwapChain _light;
	PNuoResourceSwapChain _mvp;

	PNuoRenderTarget _intermediateTarget;

	float _zoomDelta;
	float _rotationXDelta;
	float _rotationYDelta;
	float _transXDelta;
	float _transYDelta;

public:

	ModelRenderer(const PNuoCommandBuffer& commandBuffer, unsigned int frameCount,
				  std::vector<PNuoResource>& intermediate, DXGI_FORMAT format);

	PModelState State() const;

	void SetFieldOfView(float fieldOfView);

	virtual void SetDrawableSize(const NuoSize& size) override;
	virtual void SetSampleCount(unsigned int sampleCount) override;

	virtual void PredrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) override;
	virtual void DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) override;

	void HandleDeltaPosition();

	void SetRotationDelta(float dx, float dy);
	void SetTransDelta(float dx, float dy);

private:

	void UpdateUniformsForView(const PNuoCommandBuffer& commandBuffer);

};


#endif
