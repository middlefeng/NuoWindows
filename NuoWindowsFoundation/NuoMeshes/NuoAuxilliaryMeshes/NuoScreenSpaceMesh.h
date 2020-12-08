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

protected:

    virtual bool EnableDepth() override;

public:

    void Init(const PNuoCommandBuffer& commandBuffer,
              std::vector<PNuoResource>& intermediate,
              DXGI_FORMAT format, unsigned int sampleCount);

    virtual std::vector<D3D12_INPUT_ELEMENT_DESC> InputDesc() override;
    virtual PNuoRootSignature RootSignature(const PNuoCommandBuffer& commandBuffer) override;

    // screen-space meshes do not have a transform uniform. that requires an overriding
    // to the default mesh's Draw()
    //
    virtual void Draw(const PNuoCommandEncoder& encoder) override;

};


class NuoTextureMesh : public NuoScreenSpaceMesh
{

private:

    PNuoTexture _texture;
    std::vector<PNuoDescriptorHeap> _paramHeap;

public:

    NuoTextureMesh(const PNuoCommandBuffer& buffer, unsigned int frameCount);

    void SetTexture(const NuoRenderInFlight* inFlight, const PNuoTexture& texture);

    virtual bool HasTransparency() const override;
    virtual void SetTransparency(bool transparency) override;

    virtual PNuoRootSignature RootSignature(const PNuoCommandBuffer& commandBuffer) override;
    virtual void Draw(const PNuoCommandEncoder& encoder) override;

};


