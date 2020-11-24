

#include "NuoCubeMesh.h"
#include "NuoAppInstance.h"

#include "NuoDirect/NuoShader.h"
#include "NuoDirect/NuoResourceSwapChain.h"

#include "NuoStrings.h"
#include "NuoFile.h"

#include "NuoModelLoader/NuoModelLoader.h"



void NuoCubeMesh::Init(const PNuoCommandBuffer& commandBuffer,
					   std::vector<PNuoResource>& intermediate,
					   float width, float height, float depth, DXGI_FORMAT format,
					   unsigned int sampleCount)
{
	PNuoModelBoard board = std::make_shared<NuoModelBoard>(width, height, depth);
	board->CreateBuffer();

	NuoMeshSimple::Init(commandBuffer, intermediate, board, format, sampleCount);
}


