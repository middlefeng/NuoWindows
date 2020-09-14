
#include "NuoAppInstance.h"
#include "NuoStrings.h"

#include <gdiplus.h>


static NuoAppInstance* gInstance = nullptr;


NuoAppInstance::NuoAppInstance(HINSTANCE hInstance, int cmdShow)
	: _instance(hInstance),
	  _cmdShow(cmdShow)
{
}


NuoAppInstance* NuoAppInstance::GetInstance()
{
	return gInstance;
}


void NuoAppInstance::Init(HINSTANCE hInstance, int cmdShow)
{
	gInstance = new NuoAppInstance(hInstance, cmdShow);

	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	CoInitializeEx(0, COINIT_MULTITHREADED);
	OleInitialize(0);

	ULONG_PTR token;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	auto status = Gdiplus::GdiplusStartup(&token, &gdiplusStartupInput, NULL);
}


HINSTANCE NuoAppInstance::Instance()
{
	return _instance;
}


int NuoAppInstance::CommandShow()
{
	return _cmdShow;
}


std::string NuoAppInstance::ModulePath()
{
	TCHAR szFileName[MAX_PATH];
	GetModuleFileName(_instance, szFileName, MAX_PATH);

	std::string result = StringToUTF8(szFileName);
	return result;
}


void NuoAppInstance::Exit()
{
	::PostQuitMessage(0);
}