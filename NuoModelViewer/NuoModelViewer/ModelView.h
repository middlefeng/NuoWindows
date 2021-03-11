#pragma once

#include "NuoWindow.h"
#include "NuoTimer.h"

#include "NuoDirect/NuoDirectView.h"
#include "NuoModelLoader/NuoModelLoader.h"

#include <functional>
#include <DirectXMath.h>


class ModelView;
typedef std::shared_ptr<ModelView> PDirectView;

class ModelState;
typedef std::shared_ptr<ModelState> PModelState;

class NuoTextureMesh;
typedef std::shared_ptr<NuoTextureMesh> PNuoTextureMesh;

class NuoMesh;
class NuoMeshSceneRoot;
typedef std::shared_ptr<NuoMesh> PNuoMesh;
typedef std::shared_ptr<NuoMeshSceneRoot> PNuoMeshSceneRoot;



class ModelView : public NuoDirectView
{
	PModelState _modelState;
	
	PNuoTextureMesh _textureMesh;
	PNuoResourceSwapChain _light;

	PNuoRenderTarget _intermediateTarget;

	PNuoTimer _refreshTimer;

	NuoMatrixFloat44 _modelTransfer;

	bool _init;

public:

	ModelView(const PNuoDevice& device, const PNuoWindow& parent);

	void Init();
	void OpenFile(const std::string& path, NuoTaskProgress progress);
	void LoadMesh(const std::string& path, NuoTaskProgress progress);

	virtual void Render(const PNuoCommandBuffer& commandBuffer) override;
	virtual void OnSize(unsigned int x, unsigned int y) override;

	virtual void OnMouseDown(short x, short y) override;
	virtual void OnMouseDrag(short x, short y, short deltaX, short deltaY) override;

};




