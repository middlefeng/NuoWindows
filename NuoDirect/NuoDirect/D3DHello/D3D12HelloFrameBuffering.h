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

#pragma once

#include "DXSample.h"
#include "NuoDirect/NuoDirectView.h"
#include "NuoDirect/NuoSignature.h"
#include "NuoDirect/NuoPipelineState.h"

using namespace DirectX;

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

class D3D12HelloFrameBuffering : public DXSample
{
public:

    PNuoDirectView _view;

    D3D12HelloFrameBuffering(std::wstring name);

    virtual void OnInit();
    virtual void OnUpdate();
    virtual void OnRender();

private:

    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

    PNuoPipelineState _pipeline;
    
    // App resources.
    PNuoResource _vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    void LoadAssets();
    void PopulateCommandList();
};
