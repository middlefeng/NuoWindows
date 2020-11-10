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

    _mesh = std::make_shared<NuoCubeMesh>();
    _mesh->Init(commandBuffer, intermediate, 2.0, 2.0, 2.0);

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

        // for right-hand and front cull.
        //
        UINT32 indicies[] = { 0, 2, 1 };

        _vertexBuffer = std::make_shared<NuoVertexBuffer>(commandBuffer, intermediate,
                                                          triangleVertices, sizeof(triangleVertices), sizeof(Vertex),
                                                          indicies, 3);
    }

    commandBuffer->Commit();

    // Create synchronization objects and wait until assets have been uploaded to the GPU.
    {
       PNuoFenceSwapChain fence = device->CreateFenceSwapChain(1);
       fence->WaitForGPU(CommandQueue());
    }
}

struct Light
{
    DirectX::XMFLOAT4 direction;
    DirectX::XMFLOAT4 ambientColor;
    DirectX::XMFLOAT4 diffuseColor;
    DirectX::XMFLOAT4 specularColor;
};


void DirectView::Render(const PNuoCommandBuffer& commandBuffer)
{
	PNuoRenderTarget target = CurrentRenderTarget();
    PNuoCommandEncoder encoder = target->RetainRenderPassEncoder(commandBuffer);

    encoder->SetClearColor(NuoVectorFloat4(0.0f, 0.2f, 0.4f, 1.0f));
    encoder->SetViewport(NuoViewport());

	encoder->SetPipeline(_pipeline);

	auto view = target->View();
	auto vertexBufferView = _vertexBuffer->View();

	// Record commands.

    InputParamType param;
    param.color = { 1.0, 0.5, 0.0, 1.0 };
    encoder->SetRootConstant(0, sizeof(InputParamType), &param);

	encoder->SetVertexBuffer(_vertexBuffer);
	encoder->DrawIndexed(_vertexBuffer->IndiciesCount());

    const NuoVectorFloat3 rotationAxis(0, 1, 0);
    const NuoMatrixFloat44 modelMatrix = NuoMatrixRotation(rotationAxis, DirectX::XMConvertToRadians(15));

    using namespace DirectX;
    
    const NuoVectorFloat3 eyePosition(0, 0, 10);
    const NuoVectorFloat3 focusPoint(0, 0, 0);
    const NuoVectorFloat3 upDirection(0, 1, 0);

    auto viewMatrix = NuoMatrixLookAt(eyePosition, focusPoint, upDirection);

    float aspectRatio = target->Resource()->Width() / (float)target->Resource()->Height();
    NuoMatrixFloat44 projectionMatrix = NuoMatrixPerspective(aspectRatio, XMConvertToRadians(70), 0.1f, 100.f);

    NuoMatrixFloat44 mvpMatrix = viewMatrix * modelMatrix;
    NuoMatrixFloat44 normalMatrix = NuoMatrixExtractLinear(mvpMatrix);
    mvpMatrix = projectionMatrix * mvpMatrix;

    NuoModelViewProjection mvp = { mvpMatrix._m, normalMatrix._m };

    NuoMesh::CommonFunc commFunc = [&mvp](NuoCommandEncoder* encoder)
    {
        encoder->SetRootConstant(0, sizeof(NuoModelViewProjection), &mvp);
    };
    
    _mesh->DrawBegin(encoder, commFunc);
    _mesh->Draw(encoder);

	target->ReleaseRenderPassEncoder();
}



