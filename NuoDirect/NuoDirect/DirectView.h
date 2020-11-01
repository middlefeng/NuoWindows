#pragma once

#include "NuoWindow.h"
#include "NuoTimer.h"

#include "NuoDirect/NuoDirectView.h"
#include <functional>
#include <DirectXMath.h>


class DirectView;
typedef std::shared_ptr<DirectView> PDirectView;


class DirectView : public NuoDirectView
{

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		
		DirectX::XMFLOAT4 color;
	};

	PNuoPipelineState _pipeline;

	// App resources.
	PNuoVertexBuffer _vertexBuffer;

	PNuoTimer _refreshTimer;

public:

	void Init();

	DirectView(const PNuoDevice& device, const PNuoWindow& parent);

	virtual void Render(const PNuoCommandBuffer& commandBuffer) override;
	virtual void OnSize(unsigned int x, unsigned int y) override;

};




