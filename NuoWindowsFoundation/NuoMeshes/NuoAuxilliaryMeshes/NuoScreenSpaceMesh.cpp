//
//  NuoScreenSpaceMesh.m
//  ModelViewer
//
//  Created by Dong on 9/30/17.
//  Copyright © 2017 middleware. All rights reserved.
//

#include "NuoScreenSpaceMesh.h"




void NuoScreenSpaceMesh::Init(const PNuoCommandBuffer& commandBuffer,
                              std::vector<PNuoResource>& intermediate,
                              DXGI_FORMAT format, unsigned int sampleCount)
{
    NuoMeshScreenSpaceItem vertices[] =
    {
        { -1, 1,  0, 1.0 },  { 0, 0 },
        { -1, -1, 0, 1.0 },  { 0, 1 },
        { 1, -1,  0, 1.0 },  { 1, 1 },
        { 1, 1,   0, 1.0 },  { 1, 0 },
    };
    
    uint32_t indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    NuoMeshBase<NuoMeshScreenSpaceItem>::Init(commandBuffer, intermediate,
                                              vertices, sizeof(vertices) / sizeof(vertices[0]),
                                              indices, sizeof(indices) / sizeof(indices[0]));

    _format = format;
    _sampleCount = sampleCount;

    _pipelineState = MakePipelineState(commandBuffer, "NuoTextureVertex", "NuoTexturePixel");
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


void NuoTextureMesh::SetSource(const PNuoRenderTarget& source)
{
    _source = source;
}


PNuoRootSignature NuoTextureMesh::RootSignature(const PNuoCommandBuffer& commandBuffer)
{
    PNuoRootSignature rootSignature = NuoScreenSpaceMesh::RootSignature(commandBuffer);

    rootSignature->AddSampler(0, 0, D3D12_SHADER_VISIBILITY_PIXEL);
    return rootSignature;
}


void NuoTextureMesh::Draw(const PNuoCommandEncoder& encoder)
{
    if (_source)
        encoder->SetTexture(0, _source);
    
    NuoScreenSpaceMesh::Draw(encoder);
}

