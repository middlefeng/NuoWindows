#pragma once

#include "NuoWindow.h"
#include "NuoTimer.h"

#include "NuoDirect/NuoDirectView.h"
#include "NuoMeshes/NuoMesh.h"
#include "NuoMeshes/NuoCubeMesh.h"

#include <functional>
#include <DirectXMath.h>


class ModelView;
typedef std::shared_ptr<ModelView> PDirectView;


class ModelView : public NuoDirectView
{

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		
		DirectX::XMFLOAT4 color;
	};

	PNuoPipelineState _pipeline;
	PNuoVertexBuffer _vertexBuffer;

	PNuoCubeMesh _mesh;

	PNuoTimer _refreshTimer;

	NuoMatrixFloat44 _modelTransfer;

public:

	void Init();

	ModelView(const PNuoDevice& device, const PNuoWindow& parent);

	virtual void Render(const PNuoCommandBuffer& commandBuffer) override;
	virtual void OnSize(unsigned int x, unsigned int y) override;

	virtual void OnMouseDown(short x, short y) override;
	virtual void OnMouseDrag(short x, short y, short deltaX, short deltaY) override;

};




