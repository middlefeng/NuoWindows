
#pragma once

#include <Windows.h>

#include <string>
#include <functional>
#include <memory>


typedef std::function<void(void)> SimpleFunc;


class NuoMenuBar;
typedef std::shared_ptr<NuoMenuBar> PNuoMenuBar;


class NuoWindow
{
	HWND _hWnd;
	PNuoMenuBar _menu;

	std::string _title;
	SimpleFunc _onDestroy;

public:

	NuoWindow(const std::string& title);
	virtual ~NuoWindow();

	static void RegisterClass();

	virtual void OnDestroy();
	virtual bool OnCommand(int id);

	void Show();
	void Update();
	void SetIcon(const int icon);
	void SetMenu(const PNuoMenuBar& menu);

	void Destroy();
	void SetOnDestroy(SimpleFunc func);

};


typedef std::shared_ptr<NuoWindow> PNuoWindow;

