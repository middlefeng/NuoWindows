

#include "NuoMesh.h"


void NuoMesh::DrawBegin(const PNuoCommandEncoder& encoder, CommonFunc& func)
{
	encoder->SetPipeline(PipelineState());
	func(encoder.get());
}
