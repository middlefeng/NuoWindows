//
//  NotationLight.h
//  ModelViewer
//
//  Created by middleware on 11/13/16.
//  Ported: 9/23/21
//  Copyright © 2021 Dong Feng. All rights reserved.
//


#include "NuoUI/NuoRect.h"
#include "NuoMeshes/NuoMeshBounds.h"
#include "NuoShadowMap/NuoLightSource.h"
#include "NuoMeshes/NuoMesh.h"

#include <memory>
#include <vector>


class NuoCommandQueue;
typedef std::shared_ptr<NuoCommandQueue> PNuoCommandQueue;

class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;

class NuoCommandEncoder;
typedef std::shared_ptr<NuoCommandEncoder> PNuoCommandEncoder;

class NotationLightMesh;
typedef std::shared_ptr<NotationLightMesh> PNotationLightMesh;

class NuoResource;
typedef std::shared_ptr<NuoResource> PNuoResource;


class NotationLight
{
	PNuoCommandQueue _commandQueue;

	NuoLightSource _lightSourceDesc;
	PNotationLightMesh _lightVector;

	bool _selected;

public:

	NotationLight(const PNuoCommandBuffer& commandBuffer,
				  unsigned int frameCount,
				  std::vector<PNuoResource>& intermediate,
				  DXGI_FORMAT format, bool bold);

	NuoPoint<float> HeadPointProjectedWithView(const NuoMatrixFloat44& view);

	void DrawWithRenderPass(const PNuoCommandEncoder& renderPass,
							NuoMesh::CommonFunc func);

	void UpdateLightTransform(const NuoMatrixFloat44& delta);

	void SetSelected(bool selected);

	NuoMeshBounds Bounds();

private:

	void UpdatePrivateUniform(const PNuoCommandBuffer& commandBuffer,
							  std::vector<PNuoResource>& intermediate);

	void UpdateUniformsForView(const PNuoCommandEncoder& renderPass);

};


typedef std::shared_ptr<NotationLight> PNotationLight;
