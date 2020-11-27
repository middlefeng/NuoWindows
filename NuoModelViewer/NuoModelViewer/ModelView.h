#pragma once

#include "NuoWindow.h"
#include "NuoTimer.h"

#include "NuoDirect/NuoDirectView.h"
#include "NuoMeshes/NuoMesh.h"

#include <functional>
#include <DirectXMath.h>


class ModelView;
typedef std::shared_ptr<ModelView> PDirectView;

class NuoTextureMesh;
typedef std::shared_ptr<NuoTextureMesh> PNuoTextureMesh;


class ModelView : public NuoDirectView
{

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		
		DirectX::XMFLOAT4 color;
	};

	PNuoPipelineState _pipeline;
	PNuoVertexBuffer _vertexBuffer;

	PNuoMesh _mesh;
	PNuoTextureMesh _textureMesh;
	PNuoResourceSwapChain _light;

	PNuoRenderTarget _intermediateTarget;

	PNuoTimer _refreshTimer;

	NuoMatrixFloat44 _modelTransfer;

	bool _init;

public:

	void Init();

	ModelView(const PNuoDevice& device, const PNuoWindow& parent);

	virtual void Render(const PNuoCommandBuffer& commandBuffer) override;
	virtual void OnSize(unsigned int x, unsigned int y) override;

	virtual void OnMouseDown(short x, short y) override;
	virtual void OnMouseDrag(short x, short y, short deltaX, short deltaY) override;

};




