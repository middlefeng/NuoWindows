#pragma once

#include <Windows.h>
#include <string>

class NuoAppInstance
{
	HINSTANCE _instance;
	int _cmdShow;

	static ULONG_PTR _gdiToken;

	NuoAppInstance(HINSTANCE hInstance, int cmdShow);

public:

	static void Init(HINSTANCE hInstance, int cmdShow);
	static void UnInit();
	static NuoAppInstance* GetInstance();

	HINSTANCE Instance();
	std::string ModulePath();
	int CommandShow();
	static void Exit();
};

