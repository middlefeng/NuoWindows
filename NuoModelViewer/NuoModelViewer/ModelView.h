#pragma once

#include "NuoDirect/NuoDirectView.h"
#include "NuoModelLoader/NuoModelLoader.h"

#include <functional>
#include <DirectXMath.h>


class ModelView;
typedef std::shared_ptr<ModelView> PModelView;

class ModelState;
typedef std::shared_ptr<ModelState> PModelState;

class NuoTextureMesh;
typedef std::shared_ptr<NuoTextureMesh> PNuoTextureMesh;

class NuoMesh;
class NuoMeshSceneRoot;
typedef std::shared_ptr<NuoMesh> PNuoMesh;
typedef std::shared_ptr<NuoMeshSceneRoot> PNuoMeshSceneRoot;



class ModelRenderer;
typedef std::shared_ptr<ModelRenderer> PModelRenderer;

class NotationRenderer;
typedef std::shared_ptr<NotationRenderer> PNotationRenderer;



class ModelView : public NuoDirectView
{
	PNuoTimer _refreshTimer;

	bool _init;

	bool _trackingLighting;

	// renderers
	//

	PModelRenderer _modelRenderer;
	PNotationRenderer _notationRenderer;

public:

	ModelView(const PNuoDevice& device, const PNuoWindow& parent);

	PModelState State();

	void Init();
	void SetupPipelineSettings();

	void OpenFile(const std::string& path, NuoTaskProgress progress);
	void LoadMesh(const std::string& path, NuoTaskProgress progress);

	virtual void Render(const PNuoCommandBuffer& commandBuffer) override;
	virtual void OnSize(unsigned int x, unsigned int y) override;

	virtual void OnMouseDown(short x, short y) override;
	virtual void OnMouseDrag(short x, short y, short deltaX, short deltaY) override;
	virtual void OnMouseUp(short x, short y) override;

};




