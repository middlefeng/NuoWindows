#pragma once

#include <Windows.h>

class NuoAppInstance
{
	HINSTANCE _instance;
	int _cmdShow;

	NuoAppInstance(HINSTANCE hInstance, int cmdShow);

public:

	static void Init(HINSTANCE hInstance, int cmdShow);
	static NuoAppInstance* GetInstance();

	HINSTANCE Instance();
	int CommandShow();
	static void Exit();
};

