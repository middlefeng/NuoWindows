
#pragma once

#include <Windows.h>

#include <string>
#include <functional>


typedef std::function<void(void)> SimpleFunc;


class NuoWindow
{
	HWND _hWnd;

	std::string _title;
	SimpleFunc _onDestroy;

public:

	NuoWindow(const std::string& title);

	static void RegisterClass();

	virtual void OnDestroy();

	void Show();
	void Update();
	void SetIcon(const int icon);

	void SetOnDestroy(SimpleFunc func);

};

