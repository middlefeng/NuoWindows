#pragma once

#include <string>
#include <d3dcommon.h>

std::wstring StringToUTF16(const std::string& s);
std::string StringToUTF8(const std::wstring& s);
std::string RemoveLastPathComponent(const std::string& s);

std::wstring ErrorMessage(ID3DBlob* errorBlob);
