//
//  NuoScreenSpaceMesh.m
//  ModelViewer
//
//  Created by Dong on 9/30/17.
//  Copyright © 2017 middleware. All rights reserved.
//

#include "NuoScreenSpaceMesh.h"




void NuoScreenSpaceMesh::Init(const PNuoCommandBuffer& commandBuffer,
                              unsigned int frameCount,
                              std::vector<PNuoResource>& intermediate,
                              DXGI_FORMAT format)
{
    NuoMeshScreenSpaceItem vertices[] =
    {
        {{ -1,  1,  0, 1.0 },  { 0, 0 }},
        {{ -1, -1,  0, 1.0 },  { 0, 1 }},
        {{  1, -1,  0, 1.0 },  { 1, 1 }},
        {{  1,  1,  0, 1.0 },  { 1, 0 }}
    };
    
    uint32_t indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    _format = format;
    
    NuoMeshBase<NuoMeshScreenSpaceItem>::Init(commandBuffer, frameCount, intermediate,
                                              vertices, sizeof(vertices) / sizeof(vertices[0]),
                                              indices, sizeof(indices) / sizeof(indices[0]));
}


bool NuoScreenSpaceMesh::EnableDepth()
{
    return false;
}



std::vector<D3D12_INPUT_ELEMENT_DESC> NuoScreenSpaceMesh::InputDesc()
{
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs =
    {
        { "POSITION",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEX_COORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    return inputElementDescs;
}


PNuoRootSignature NuoScreenSpaceMesh::RootSignature(const PNuoCommandBuffer& commandBuffer)
{
    PNuoDevice device = commandBuffer->CommandQueue()->Device();

    PNuoRootSignature rootSignature = std::make_shared<NuoRootSignature>(device,
                                                                         D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    return rootSignature;
}


void NuoScreenSpaceMesh::MakePipelineState(const PNuoCommandBuffer& commandBuffer)
{
    _pipelineState = NuoMesh::MakePipelineState(commandBuffer, "NuoTextureVertex", "NuoTexturePixel");
}


void NuoScreenSpaceMesh::Draw(const PNuoCommandEncoder& encoder)
{
    encoder->SetVertexBuffer(_vertexBuffer);
    encoder->DrawIndexed(_vertexBuffer->IndiciesCount());
}


NuoTextureMesh::NuoTextureMesh(const PNuoCommandBuffer& buffer, unsigned int frameCount)
{
    _paramHeap.resize(frameCount);
    
    for (auto& heap : _paramHeap)
        heap = buffer->CommandQueue()->Device()->CreateShaderDescriptorHeap(1);
}


void NuoTextureMesh::SetTexture(const NuoRenderInFlight* inFlight, const PNuoTexture& texture)
{
    unsigned int flight = 0;
    
    // respect the in-flight index only if the number of the param buffers is not 1.
    // otherwise the param is a pure-GPU buffer and does not need tri-buffer rotation.
    //
    if (_paramHeap.size() != 1 && inFlight)
        flight = inFlight->InFlight();

    if (_paramHeap.size() != 1 || _texture != texture)
    {
        _paramHeap[flight]->SetTexture(0, texture);
    }

    _texture = texture;
}


bool NuoTextureMesh::HasTransparency() const
{
    return false;
}


void NuoTextureMesh::SetTransparency(bool transparency)
{
}


PNuoRootSignature NuoTextureMesh::RootSignature(const PNuoCommandBuffer& commandBuffer)
{
    PNuoRootSignature rootSignature = NuoScreenSpaceMesh::RootSignature(commandBuffer);

    rootSignature->AddSampler(0, 0, D3D12_SHADER_VISIBILITY_PIXEL);
    int indexOfTable = rootSignature->AddDescriptorTable(1 /* range - one range of textures */, D3D12_SHADER_VISIBILITY_PIXEL);

    rootSignature->AddTexturesToDescriptorTable(indexOfTable, 0 /* first range */,
                                                1 /* one texture */,
                                                0, 0 /* register t0, space 0 */);
    return rootSignature;
}


void NuoTextureMesh::Draw(const PNuoCommandEncoder& encoder)
{
    if (_texture)
    {
        unsigned int flight = 0;
        if (_paramHeap.size() != 1)
            flight = encoder->InFlight();

        encoder->SetDescriptorTable(0, _paramHeap[flight]);
    }
    
    NuoScreenSpaceMesh::Draw(encoder);
}

