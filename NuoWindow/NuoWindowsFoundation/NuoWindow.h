
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

class NuoIcon;
typedef std::shared_ptr<NuoIcon> PNuoIcon;


class NuoFont : public std::enable_shared_from_this<NuoFont>
{
	HFONT _hFont;

	std::string _name;
	double _weight;
	bool _isItalic;
	bool _isLight;

	bool _fontOwner;

public:
	NuoFont(HFONT _hFont);
	NuoFont(double weight, const std::string& name);
	~NuoFont();

	void CreateFont(float scale);

	void SetLight(bool b);
	void SetItalic(bool b);

	HFONT Handle() const;
};


class NuoWindow : public std::enable_shared_from_this<NuoWindow>
{

protected:

	HWND _hWnd;
	PNuoMenuBar _menu;
	bool _inDPIChange;
	float _savedDPI;

	std::string _title;
	SimpleFunc _onDestroy;

	std::set<PNuoWindow> _children;
	PNuoIcon _icon;

	std::shared_ptr<NuoFont> _font;

public:

	NuoWindow();
	NuoWindow(const std::string& title);
	virtual ~NuoWindow();

	static void RegisterClass();

	virtual void OnDestroy();
	virtual bool OnCommand(int id);
	virtual void OnSize(unsigned int x, unsigned int y);
	virtual void OnDPIChange(const NuoRect<long>& newRect, float newDPI, float oldDPI);

	HWND Handle() const;

	void Show();
	void Hide();
	void Update();
	
	void SetMenu(const PNuoMenuBar& menu);
	void SetIcon(const PNuoIcon& icon);
	virtual NuoRect<long> PositionDevice();
	void SetPositionDevice(const NuoRect<long>& pos, bool activate);
	NuoRect<long> ClientRect();
	float DPI() const;

	std::shared_ptr<NuoFont> Font();
	void SetFont(const std::shared_ptr<NuoFont>& font);

	void Detach();
	void Destroy();
	void SetOnDestroy(SimpleFunc func);

	void Add(const PNuoWindow& child);

private:

	float SavedDPI() const;

	static LRESULT CALLBACK NuoWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};



