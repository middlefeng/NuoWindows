

#include "NuoShader.h"
#include "NuoStrings.h"

#include <cassert>

NuoShader::NuoShader(const std::string& source,
					 const std::string& name,
	                 NuoShader::Type type, const std::string entryPoint)
{
	Microsoft::WRL::ComPtr<IDxcLibrary> library;
	HRESULT hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcCompiler> compiler;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
	assert(hr == S_OK);

	uint32_t codePage = CP_UTF8;
	Microsoft::WRL::ComPtr<IDxcBlobEncoding> sourceBlob;
	hr = library->CreateBlobWithEncodingFromPinned(source.c_str(), source.size(), codePage, &sourceBlob);
	assert(hr == S_OK);

	std::wstring wName = StringToUTF16(name);
	std::wstring wEntry = StringToUTF16(entryPoint);

	std::wstring targetProfile = L"vs_6_3";
	if (type == NuoShader::kNuoShader_Pixel)
		targetProfile = L"ps_6_3";
	else if (type == NuoShader::kNuoShader_Lib)
		targetProfile = L"lib_6_3";

	Microsoft::WRL::ComPtr<IDxcOperationResult> result;
	hr = compiler->Compile(sourceBlob.Get(), wName.c_str(),
						   wEntry.c_str(), // pEntryPoint
						   targetProfile.c_str(), // pTargetProfile
						   NULL, 0, // pArguments, argCount
						   NULL, 0, // pDefines, defineCount
						   NULL, // pIncludeHandler
						   &result); // ppResult

#if defined(_DEBUG)
	HRESULT status;
	result->GetStatus(&status);

	if (status != S_OK)
	{
		Microsoft::WRL::ComPtr<IDxcBlobEncoding> errorsBlob;
		hr = result->GetErrorBuffer(&errorsBlob);
		char message[1024];

		if (SUCCEEDED(hr) && errorsBlob)
		{
			sprintf_s(message, "%s", (const char*)errorsBlob->GetBufferPointer());
		}

		std::wstring wstr = StringToUTF16(message);
		OutputDebugString(wstr.c_str());
		
		assert(false);
	}
#endif

	result->GetResult(&_dxShaderBlob);
}

D3D12_SHADER_BYTECODE NuoShader::ByteCode()
{
	D3D12_SHADER_BYTECODE result;
	result.BytecodeLength = _dxShaderBlob->GetBufferSize();
	result.pShaderBytecode = _dxShaderBlob->GetBufferPointer();

	return result;
}

