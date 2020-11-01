

#include "NuoCommandQueue.h"
#include "NuoStrings.h"

#include <windows.h>



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



ID3D12CommandQueue* NuoCommandQueue::DxQueue() const
{
    return _dxQueue.Get();
}
