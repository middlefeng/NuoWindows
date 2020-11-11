

#include "NuoPipelineState.h"
#include "NuoShader.h"
#include "NuoStrings.h"

#include <cassert>


NuoPipelineState::NuoPipelineState(const PNuoDevice& device,
                                   DXGI_FORMAT format,
                                   const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputDesc,
								   const PNuoShader& vertex,
								   const PNuoShader& pixel,
                                   const PNuoRootSignature& rootSignature)
    : _rootSignature(rootSignature)
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputDesc.data(),  (UINT) inputDesc.size() };
    psoDesc.pRootSignature = _rootSignature->DxSignature();
    psoDesc.VS = vertex->ByteCode();
    psoDesc.PS = pixel->ByteCode();

    D3D12_DEPTH_STENCIL_DESC depthDesc;
    depthDesc.DepthEnable = true;
    depthDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    depthDesc.StencilEnable = false;
    depthDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    depthDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
    const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
    {
        D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP,
        D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS
    };
    depthDesc.FrontFace = defaultStencilOp; // both front and back facing polygons get the same treatment
    depthDesc.BackFace = defaultStencilOp;
    psoDesc.DepthStencilState = depthDesc;

    D3D12_RASTERIZER_DESC rasterizerDesc;
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizerDesc.CullMode = D3D12_CULL_MODE_FRONT;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;
    rasterizerDesc.ForcedSampleCount = 0;
    rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    psoDesc.RasterizerState = rasterizerDesc;

    D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
    renderTargetBlendDesc.BlendEnable = false;
    renderTargetBlendDesc.LogicOpEnable = false;
    renderTargetBlendDesc.SrcBlend = D3D12_BLEND_ONE;
    renderTargetBlendDesc.DestBlend = D3D12_BLEND_ZERO;
    renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
    renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
    renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    renderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
    renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    D3D12_BLEND_DESC blendDesc;
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        blendDesc.RenderTarget[i] = renderTargetBlendDesc;

    psoDesc.BlendState = blendDesc;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = format;
    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleDesc.Count = 1;

    HRESULT hr = device->DxDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&_dxPipelineState));
    assert(hr == S_OK);
}


ID3D12PipelineState* NuoPipelineState::DxPipeline() const
{
    return _dxPipelineState.Get();
}


ID3D12RootSignature* NuoPipelineState::DxRootSignature() const
{
    if (!_rootSignature)
        return nullptr;
    return _rootSignature->DxSignature();
}


