
#pragma once

#include <Windows.h>

#include <string>
#include <set>
#include <functional>
#include <memory>

#include "NuoRect.h"


extern const int kWindowPtr;

typedef std::function<void(void)> SimpleFunc;


class NuoMenuBar;
typedef std::shared_ptr<NuoMenuBar> PNuoMenuBar;

class NuoWindow;
typedef std::shared_ptr<NuoWindow> PNuoWindow;


class NuoFont
{
	HFONT _hFont;

public:
	NuoFont(HFONT _hFont);
	HFONT Handle() const;
};


class NuoWindow : public std::enable_shared_from_this<NuoWindow>
{

protected:

	HWND _hWnd;
	PNuoMenuBar _menu;

	std::string _title;
	SimpleFunc _onDestroy;

	std::set<PNuoWindow> _children;

public:

	NuoWindow();
	NuoWindow(const std::string& title);
	virtual ~NuoWindow();

	static void RegisterClass();

	virtual void OnDestroy();
	virtual bool OnCommand(int id);

	HWND Handle() const;

	void Show();
	void Update();
	void SetIcon(const int icon);
	void SetMenu(const PNuoMenuBar& menu);

	virtual NuoRect<long> PositionDevice();
	void SetPositionDevice(const NuoRect<long>& pos, bool activate);
	NuoRect<long> ClientRect();
	float DPI();

	NuoFont Font();
	void SetFont(const NuoFont& font);

	void Detach();
	void Destroy();
	void SetOnDestroy(SimpleFunc func);

	void Add(const PNuoWindow& child);

};



