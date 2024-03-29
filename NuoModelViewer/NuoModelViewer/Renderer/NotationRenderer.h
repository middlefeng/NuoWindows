


//
//  NotationRenderer.h
//  ModelViewer
//
//  Created by Dong Feng on 12/23/21.
//  Copyright � 2021 Dong Feng. All rights reserved.
//


#ifndef __NOTATION_RENDERER_H__
#define __NOTATION_RENDERER_H__

#pragma once

#include "NuoRender/NuoRenderPipelinePass.h"

#include "NuoUtilites/NuoMathVector.h"

#include "NotationLight.h"
#include "NuoRect.h"

#include <dxgi1_6.h>
#include <memory>


class NuoRenderInFlight;
typedef std::shared_ptr<NuoRenderInFlight> PNuoRenderInFlight;



class NotationRenderer : public NuoRenderPipelinePass
{

	std::vector<PNotationLight> _lightVectors;
	std::weak_ptr<NotationLight> _currentLightVector;

	// light to illuminate the notations
	//
	PNuoResourceSwapChain _lightBuffer;
	PNuoResourceSwapChain _transforms;

	PNuoRenderTarget _resolvedTarget;

	float _notationWidthCap;
	NuoRect<float> _notationArea;
	float _dpiFactor;

public:

	NotationRenderer(const PNuoCommandBuffer& commandBuffer,
					 unsigned int frameCount,
					 std::vector<PNuoResource>& intermediate,
					 DXGI_FORMAT format);

	void SelectCurrentLightVector(const NuoPoint<short>& point);

	virtual void SetDrawableSize(const NuoSize& size) override;
	virtual void SetSampleCount(unsigned int sampleCount) override;

	virtual void PredrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) override;
	virtual void DrawWithCommandBuffer(const PNuoCommandBuffer& commandBuffer) override;

	void SetNotationWidthCap(float cap);
	void SetDPI(float dpi);

	NuoRect<float> NotationArea() const;


	// manipulator to the current selected light source

	void UpdateRotation(float deltaX, float deltaY);

private:

	void UpdateUniformsForView(const PNuoRenderInFlight& inFlight);

};


#endif
