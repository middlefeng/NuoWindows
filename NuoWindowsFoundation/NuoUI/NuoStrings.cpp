#include "NuoStrings.h"

#include <Windows.h>

#include <vector>
#include <stdio.h>


std::wstring StringToUTF16(const std::string& s)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
	wchar_t* result = new wchar_t[size];

	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, result, size);
	std::wstring resultStr(result);

	delete[] result;

	return resultStr;
}



std::string StringToUTF8(const std::wstring& s)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* result = new char[size];

	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, result, size, nullptr, nullptr);
	std::string resultStr(result);

	delete[] result;

	return resultStr;
}


std::string LastPathComponent(const std::string& s)
{
	size_t delimitor = s.find_last_of("\\");
	return s.substr(delimitor + 1);
}


std::string RemoveLastPathComponent(const std::string& s)
{
	size_t delimitor = s.find_last_of("\\");
	return s.substr(0, delimitor);
}


std::wstring ErrorMessage(ID3DBlob* errorBlob)
{
	std::vector<char> message;
	message.resize(1024, '\0');

	sprintf_s(message.data(), message.size() - 1, "%s", (const char*)errorBlob->GetBufferPointer());

	return StringToUTF16(message.data());
}
