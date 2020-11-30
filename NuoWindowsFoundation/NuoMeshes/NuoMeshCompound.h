#pragma once

#include <memory>
#include <vector>


#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "NuoMesh.h"



class NuoCommandBuffer;
typedef std::shared_ptr<NuoCommandBuffer> PNuoCommandBuffer;



class NuoMeshCompound : public NuoMesh
{

protected:

	std::vector<PNuoMesh> _meshes;
	
public:

	NuoMeshCompound(const std::vector<PNuoMesh>& meshes);

};

