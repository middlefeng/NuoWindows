﻿

#include "NuoCommandQueue.h"
#include "NuoCommandBuffer.h"
#include "NuoStrings.h"

#include <windows.h>
#include <cassert>



NuoCommandQueue::NuoCommandQueue(const PNuoDevice& device)
    : _device(device)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    _device->DxDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_dxQueue));
}


PNuoDevice NuoCommandQueue::Device() const
{
    return _device;
}


void NuoCommandQueue::ReleasePendingCommandBuffers()
{
    _pendingCommandBuffers.clear();
}


PNuoCommandBuffer NuoCommandQueue::CreateCommandBuffer()
{
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator;

    HRESULT hr = Device()->DxDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                              IID_PPV_ARGS(&allocator));
    assert(hr == S_OK);

    PNuoCommandBuffer commandBuffer = std::make_shared<NuoCommandBuffer>();
    commandBuffer->_commandAllocator = allocator;
    commandBuffer->_commandQueue = shared_from_this();
    
    // this is not an in-flight command buffer
    commandBuffer->SetInFlight(0, 0);

    _pendingCommandBuffers.push_back(commandBuffer);

    return commandBuffer;
}



ID3D12CommandQueue* NuoCommandQueue::DxQueue() const
{
    return _dxQueue.Get();
}
