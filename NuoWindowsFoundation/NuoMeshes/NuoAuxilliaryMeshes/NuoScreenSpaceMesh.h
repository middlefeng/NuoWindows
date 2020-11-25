//
//  NuoScreenSpaceMesh.h
//  ModelViewer
//
//  Created by Dong on 9/30/17.
//  Copyright © 2017 middleware. All rights reserved.
//

#include "NuoMeshes/NuoMesh.h"
#include "NuoMeshes/NuoShaders/NuoMeshScreenSpace.h"


typedef enum
{
    kBlend_Alpha,
    kBlend_Accumulate,
    kBlend_None
}
ScreenSpaceBlendMode;


class NuoScreenSpaceMesh : public NuoMeshBase<NuoMeshScreenSpaceItem>
{

public:

    void Init(const PNuoCommandBuffer& commandBuffer,
              std::vector<PNuoResource>& intermediate,
              DXGI_FORMAT format, unsigned int sampleCount);

    virtual std::vector<D3D12_INPUT_ELEMENT_DESC> InputDesc() override;
    virtual PNuoRootSignature RootSignature(const PNuoCommandBuffer& commandBuffer) override;

};


class NuoTextureMesh : public NuoScreenSpaceMesh
{

private:

    PNuoResource _texture;
    PNuoRenderTarget _source;

public:

    void SetSource(const PNuoRenderTarget& source);

    virtual PNuoRootSignature RootSignature(const PNuoCommandBuffer& commandBuffer) override;
    virtual void Draw(const PNuoCommandEncoder& encoder) override;

};


