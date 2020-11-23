

#include "NuoShader.h"
#include "NuoStrings.h"

#include <cassert>
#include "NuoFile.h"
#include "NuoAppInstance.h"


class NuoShaderIncludeHandler : public IDxcIncludeHandler
{

	ULONG _ref;

public:

	NuoShaderIncludeHandler();

	virtual HRESULT QueryInterface(REFIID riid, void** ppvObject);
	virtual ULONG AddRef(void);
	virtual ULONG Release(void);

	virtual HRESULT LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource);
};


NuoShaderIncludeHandler::NuoShaderIncludeHandler()
	: _ref(1)
{
}


HRESULT NuoShaderIncludeHandler::QueryInterface(REFIID riid, void** ppvObject)
{
	*ppvObject = this;
	
	return S_OK;
}

ULONG NuoShaderIncludeHandler::AddRef(void)
{
	return ++_ref;
}

ULONG NuoShaderIncludeHandler::Release(void)
{
	auto ref = --_ref;

	if (_ref == 0)
		delete this;

	return ref;
}

HRESULT NuoShaderIncludeHandler::LoadSource(LPCWSTR pFilename, IDxcBlob** ppIncludeSource)
{
	Microsoft::WRL::ComPtr<IDxcLibrary> library;
	HRESULT hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcIncludeHandler> defaultHandler;
	library->CreateIncludeHandler(&defaultHandler);

	std::wstring filename = pFilename;
	if (filename.find_first_of(L"./") == 0)
		filename = L"\\NuoShaders\\" + filename.substr(2);

	std::string path = NuoAppInstance::GetInstance()->ModulePath();
	path = RemoveLastPathComponent(path) + StringToUTF8(filename);

	return defaultHandler->LoadSource(StringToUTF16(path).c_str(), ppIncludeSource);
}



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
	hr = library->CreateBlobWithEncodingFromPinned(source.c_str(), (UINT32)source.size(), codePage, &sourceBlob);
	assert(hr == S_OK);

	std::wstring wName = StringToUTF16(name);
	std::wstring wEntry = StringToUTF16(entryPoint);

	std::wstring targetProfile = L"vs_6_3";
	if (type == NuoShader::kNuoShader_Pixel)
		targetProfile = L"ps_6_3";
	else if (type == NuoShader::kNuoShader_Lib)
		targetProfile = L"lib_6_3";

	
	Microsoft::WRL::ComPtr<NuoShaderIncludeHandler> includeHandler;
	includeHandler.Attach(new NuoShaderIncludeHandler());

	Microsoft::WRL::ComPtr<IDxcOperationResult> result;
	
	hr = compiler->Compile(sourceBlob.Get(), wName.c_str(),
						   wEntry.c_str(),			// pEntryPoint
						   targetProfile.c_str(),	// pTargetProfile
						   NULL, 0,					// pArguments, argCount
						   NULL, 0,					// pDefines, defineCount
						   includeHandler.Get(),	// pIncludeHandler
						   &result);				// ppResult

#if defined(_DEBUG)
	HRESULT status;
	result->GetStatus(&status);

	if (status != S_OK)
	{
		Microsoft::WRL::ComPtr<IDxcBlobEncoding> errorsBlob;
		hr = result->GetErrorBuffer(&errorsBlob);
		char message[2048];

		if (SUCCEEDED(hr) && errorsBlob)
		{
			snprintf(message, 2048, "%s", (const char*)errorsBlob->GetBufferPointer());
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

