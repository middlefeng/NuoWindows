#include "NuoAppInstance.h"


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
}


HINSTANCE NuoAppInstance::Instance()
{
	return _instance;
}


int NuoAppInstance::CommandShow()
{
	return _cmdShow;
}


void NuoAppInstance::Exit()
{
	::PostQuitMessage(0);
}