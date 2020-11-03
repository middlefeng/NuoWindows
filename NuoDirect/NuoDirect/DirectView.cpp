// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "DirectView.h"

#include "NuoAppInstance.h"
#include "NuoFile.h"
#include "NuoStrings.h"

#include <dxcapi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>



struct InputParamType
{
    DirectX::XMFLOAT4 color;
};


DirectView::DirectView(const PNuoDevice& device,
					   const PNuoWindow& parent)
	: NuoDirectView(device, parent)
{
	_refreshTimer = std::make_shared<NuoTimer>(1000 / 60, [this](NuoTimer*)
		{
			this->Update();
		});
}


void DirectView::OnSize(unsigned int x, unsigned int y)
{
	NuoDirectView::OnSize(x, y);
	
    Init();
}


void DirectView::Init()
{
    PNuoDevice device = CommandQueue()->Device();

    auto rootSignature = std::make_shared<NuoRootSignature>(device,
                                                            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    rootSignature->AddConstant(sizeof(InputParamType), 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);

    // Create the pipeline state, which includes compiling and loading shaders.
    {

#if defined(_DEBUG)
        // Enable better shader debugging with the graphics debugging tools.
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif

        std::string modulePath = RemoveLastPathComponent(NuoAppInstance::GetInstance()->ModulePath());
        std::string path = modulePath + "\\" + "shaders.hlsl";

        std::vector<char> vertexContent;
        NuoFile vertexSource(path);
        vertexSource.ReadTo(vertexContent);
        PNuoShader vertexShader = std::make_shared<NuoShader>(vertexContent.data(), "vertex", NuoShader::kNuoShader_Vertex, "VSMain");

        std::vector<char> pixelxContent;
        NuoFile pixelSource(path);
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

    std::vector<PNuoResource> intermediate;
    PNuoCommandBuffer commandBuffer = CommandQueue()->CreateCommandBuffer();

    NuoRect<long> rect = ClientRectDevice();
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

        UINT32 indicies[] = { 0, 1, 2 };

        _vertexBuffer = std::make_shared<NuoVertexBuffer>(commandBuffer, intermediate,
                                                          triangleVertices, sizeof(triangleVertices), sizeof(Vertex),
                                                          indicies, 3);
        commandBuffer->Commit();
    }

    // Create synchronization objects and wait until assets have been uploaded to the GPU.
    {
       PNuoFenceSwapChain fence = device->CreateFenceSwapChain(1);
       fence->WaitForGPU(CommandQueue());
    }
}


void DirectView::Render(const PNuoCommandBuffer& commandBuffer)
{
	PNuoRenderTarget target = CurrentRenderTarget();
    PNuoCommandEncoder encoder = target->RetainRenderPassEncoder(commandBuffer);

    encoder->SetClearColor(NuoVector4(0.0f, 0.2f, 0.4f, 1.0f));
    encoder->SetViewport(NuoViewport());

	encoder->SetPipeline(_pipeline);

	auto view = target->View();
	auto vertexBufferView = _vertexBuffer->View();

	// Record commands.

    InputParamType param;
    param.color = { 1.0, 0.5, 0.0, 1.0 };

    encoder->SetConstant(0, sizeof(InputParamType), &param);
	encoder->SetVertexBuffer(_vertexBuffer);
	encoder->DrawIndexed(_vertexBuffer->IndiciesCount());

	target->ReleaseRenderPassEncoder();
}



