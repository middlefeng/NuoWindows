

#include "NuoPipelineState.h"
#include "NuoShader.h"
#include "NuoStrings.h"

#include <cassert>


NuoPipelineState::NuoPipelineState(const PNuoDevice& device,
                                   DXGI_FORMAT format,
                                   bool depthEnabled, bool depthWrite,
                                   unsigned int sampleCount,
                                   NuoBlendingMode blendingMode,
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
    depthDesc.DepthEnable = depthEnabled;
    depthDesc.DepthWriteMask = depthWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
    depthDesc.DepthFunc = depthEnabled ? D3D12_COMPARISON_FUNC_LESS : D3D12_COMPARISON_FUNC_ALWAYS;
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

    D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
    renderTargetBlendDesc.BlendEnable = false;
    renderTargetBlendDesc.LogicOpEnable = false;
    renderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
    renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    if (blendingMode != kNuoBlending_None)
    {
        renderTargetBlendDesc.BlendEnable = true;

        if (blendingMode == kNuoBlending_Accumulate)
        {
            renderTargetBlendDesc.SrcBlend = D3D12_BLEND_ONE;
        }
        else if (blendingMode == kNuoBlending_Alpha)
        {
            renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
        }
        else
        {
            renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
            assert(false);
        }
        
        renderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
        renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;

        renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
        renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
        renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
    }

    D3D12_BLEND_DESC blendDesc;
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        blendDesc.RenderTarget[i] = renderTargetBlendDesc;

    psoDesc.BlendState = blendDesc;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // is this affecting performance by setting the targets number larger
    // than actual?
    //
    psoDesc.NumRenderTargets = D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT;
    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        psoDesc.RTVFormats[i] = format;

    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleDesc.Count = sampleCount;

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


