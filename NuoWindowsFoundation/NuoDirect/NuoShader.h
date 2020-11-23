#pragma once

#include <memory>
#include <vector>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include <dxcapi.h>

#include "NuoDevice.h"

class NuoShader;
typedef std::shared_ptr<NuoShader> PNuoShader;
typedef std::weak_ptr<NuoShader> WPNuoShader;


class NuoShader
{

	Microsoft::WRL::ComPtr<IDxcBlob> _dxShaderBlob;


public:

	enum Type
	{
		kNuoShader_Vertex,
		kNuoShader_Pixel,
		kNuoShader_Lib,
	};

	NuoShader(const std::string& source,
			  const std::string& name,
			  NuoShader::Type type, const std::string entryPoint);

	D3D12_SHADER_BYTECODE ByteCode();

};

