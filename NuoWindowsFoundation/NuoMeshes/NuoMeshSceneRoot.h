#pragma once

#include <memory>
#include <vector>


#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoMeshCompound.h"



class NuoMesh;
typedef std::shared_ptr<NuoMesh> PNuoMesh;



class NuoMeshSceneRoot : public NuoMeshCompound
{

protected:
	
public:

	NuoMeshSceneRoot();
	NuoMeshSceneRoot(const std::vector<PNuoMesh>& meshes);

	void ReplaceMesh(const PNuoMesh& older, const PNuoMesh& replacer);

};

