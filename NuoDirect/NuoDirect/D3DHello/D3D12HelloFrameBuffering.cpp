//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

//#include "stdafx.h"
#include "D3D12HelloFrameBuffering.h"
#include "NuoDirect/NuoDevice.h"
#include "NuoDirect/NuoShader.h"
#include "NuoFile.h"
#include "NuoStrings.h"

#include <wrl.h>
#include <dxcapi.h>

D3D12HelloFrameBuffering::D3D12HelloFrameBuffering(std::wstring name) :
    DXSample(name)
{
}

void D3D12HelloFrameBuffering::OnInit()
{
    LoadAssets();
}

// Load the sample assets.
void D3D12HelloFrameBuffering::LoadAssets()
{
    PNuoDevice device = _view->CommandQueue()->Device();

    auto rootSignature = std::make_shared<NuoRootSignature>(device,
                                                        std::vector<D3D12_ROOT_PARAMETER1>(),
                                                        std::vector< D3D12_STATIC_SAMPLER_DESC>(),
                                                        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // Create the pipeline state, which includes compiling and loading shaders.
    {

#if defined(_DEBUG)
        // Enable better shader debugging with the graphics debugging tools.
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif

        std::vector<char> vertexContent;
        NuoFile vertexSource(StringToUTF8(GetAssetFullPath(L"shaders.hlsl")));
        vertexSource.ReadTo(vertexContent);
        PNuoShader vertexShader = std::make_shared<NuoShader>(vertexContent.data(), "vertex", NuoShader::kNuoShader_Vertex, "VSMain");

        std::vector<char> pixelxContent;
        NuoFile pixelSource(StringToUTF8(GetAssetFullPath(L"shaders.hlsl")));
        pixelSource.ReadTo(pixelxContent);
        PNuoShader pixelShader = std::make_shared<NuoShader>(pixelxContent.data(), "pixel", NuoShader::kNuoShader_Pixel, "PSMain");
        

        // Define the vertex input layout.
        std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        _pipeline = std::make_shared<NuoPipelineState>(device, DXGI_FORMAT_R8G8B8A8_UNORM, inputElementDescs, vertexShader, pixelShader, rootSignature);
    }

    PNuoResource intermediate;
    PNuoCommandBuffer commandBuffer = _view->CreateCommandBuffer();

    NuoRect<long> rect = _view->ClientRectDevice();
    float aspectRatio = ((float)rect.W()) / ((float)rect.H());

    

    // Create the vertex buffer.
    {
        // Define the geometry for a triangle.
        Vertex triangleVertices[] =
        {
            { { 0.0f, 0.25f * aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.25f, -0.25f * aspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
            { { -0.25f, -0.25f * aspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
        };

        const UINT vertexBufferSize = sizeof(triangleVertices);

        intermediate = device->CreateBuffer(triangleVertices, vertexBufferSize);
        auto vertexBuffer = device->CreateBuffer(vertexBufferSize);
        
        commandBuffer->CopyResource(intermediate, vertexBuffer);
        commandBuffer->Commit();

        _vertexBuffer = std::make_shared<NuoVertexBuffer>(vertexBuffer, sizeof(Vertex));
    }

    // Create synchronization objects and wait until assets have been uploaded to the GPU.
    {
        PNuoFenceSwapChain fence = device->CreateFenceSwapChain(1);
        fence->WaitForGPU(_view->CommandQueue());
    }
}

// Update frame-based values.
void D3D12HelloFrameBuffering::OnUpdate()
{
}

// Render the scene.
void D3D12HelloFrameBuffering::OnRender()
{
    PNuoCommandQueue queue = _view->CommandQueue();
    
    // Record all the commands we need to render the scene into the command list.
    PopulateCommandList();

    // Present the frame.
    _view->MoveToNextFrame();
}


void D3D12HelloFrameBuffering::PopulateCommandList()
{
    PNuoRenderTarget target = _view->CurrentRenderTarget();
    PNuoCommandBuffer commandBuffer = _view->CreateCommandBuffer();
    PNuoCommandEncoder encoder = target->RetainRenderPassEncoder(commandBuffer);

    encoder->SetPipeline(_pipeline);
    encoder->UseDefaultViewPort();

    auto view = target->View();
    auto vertexBufferView = _vertexBuffer->View();
    
    // Record commands.
    
    encoder->ClearTargetView(0.0f, 0.2f, 0.4f, 1.0f);
    encoder->SetVertexBuffer(_vertexBuffer);
    encoder->DrawInstanced(_vertexBuffer->Count(), 1);

    target->ReleaseRenderPassEncoder();
    commandBuffer->Commit();
}
