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

    PNuoRenderTarget renderTarget = _view->CurrentRenderTarget();
    PNuoCommandBuffer commandBuffer = _view->CreateCommandBuffer();
    PNuoCommandEncoder encoder = renderTarget->RetainRenderPassEncoder(commandBuffer);
    encoder->SetPipeline(nullptr);

    NuoRect<long> rect = _view->ClientRectDevice();
    float aspectRatio = ((float)rect.W()) / ((float)rect.H());

    ComPtr<ID3D12Resource> intermediate;

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

        // Note: using upload heaps to transfer static data like vert buffers is not 
        // recommended. Every time the GPU needs it, the upload heap will be marshalled 
        // over. Please read up on Default Heap usage. An upload heap is used here for 
        // code simplicity and because there are very few verts to actually transfer.
        ThrowIfFailed(device->DxDevice()->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&intermediate)));

        // Copy the triangle data to the vertex buffer.
        UINT8* pVertexDataBegin;
        CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
        ThrowIfFailed(intermediate->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
        memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
        intermediate->Unmap(0, nullptr);

        ThrowIfFailed(device->DxDevice()->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
            D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr,
            IID_PPV_ARGS(&m_vertexBuffer)));

        ComPtr<ID3D12GraphicsCommandList> commandList;
        commandList = encoder->CommandList();
        commandList->CopyResource(m_vertexBuffer.Get(), intermediate.Get());
        commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
        
        renderTarget->ReleaseRenderPassEncoder();
        commandBuffer->Commit();

        // Initialize the vertex buffer view.
        m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
        m_vertexBufferView.StrideInBytes = sizeof(Vertex);
        m_vertexBufferView.SizeInBytes = vertexBufferSize;
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
    // Command list allocators can only be reset when the associated 
    // command lists have finished execution on the GPU; apps should use 
    // fences to determine GPU execution progress.
    // ThrowIfFailed(_view->CurrentCommandAllocator()/*m_commandAllocators[_view->CurrentBackBufferIndex()]*/->Reset());

    // However, when ExecuteCommandList() is called on a particular command 
    // list, that command list can then be reset at any time and must be before 
    // re-recording.
    // ThrowIfFailed(m_commandList->Reset(_view->CurrentCommandAllocator()/*m_commandAllocators[_view->CurrentBackBufferIndex()].Get()*/, _pipeline->DxPipeline()));
    PNuoRenderTarget target = _view->CurrentRenderTarget();
    PNuoCommandBuffer commandBuffer = _view->CreateCommandBuffer();
    PNuoCommandEncoder encoder = target->RetainRenderPassEncoder(commandBuffer);

    encoder->SetPipeline(_pipeline);
    encoder->UseDefaultViewPort();
    auto m_commandList = encoder->CommandList();

    auto view = target->View();
    
    // Record commands.
    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_commandList->ClearRenderTargetView(view, clearColor, 0, nullptr);
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    m_commandList->DrawInstanced(3, 1, 0, 0);

    target->ReleaseRenderPassEncoder();
    commandBuffer->Commit();
}
