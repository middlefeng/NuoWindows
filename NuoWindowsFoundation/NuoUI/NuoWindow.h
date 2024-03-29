
#pragma once

#include <Windows.h>

#include <string>
#include <set>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoTimer.h"
#include "NuoMouse.h"


extern const int kWindowPtr;
extern wchar_t kClassName[];

typedef std::function<void(void)> SimpleFunc;


class NuoMenuBar;
typedef std::shared_ptr<NuoMenuBar> PNuoMenuBar;

class NuoWindow;
typedef std::shared_ptr<NuoWindow> PNuoWindow;

class NuoIcon;
typedef std::shared_ptr<NuoIcon> PNuoIcon;


class NuoFont;
typedef std::shared_ptr<NuoFont> PNuoFont;

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
	NuoFont(const NuoFont& font);
	NuoFont(double weight, const std::string& name);
	~NuoFont();

	void CreateFont(float scale);

	void SetLight(bool b);
	void SetItalic(bool b);

	HFONT Handle() const;

	static PNuoFont MenuFont(double size);
};


class NuoWindow : public std::enable_shared_from_this<NuoWindow>
{

protected:

	HWND _hWnd;
	PNuoMenuBar _menu;
	bool _inDPIChange;
	bool _inDragging;
	float _savedDPI;

	std::string _title;
	SimpleFunc _onDestroy;

	std::set<PNuoWindow> _children;
	PNuoIcon _icon;

	PNuoFont _font;

	/**
	 *   used by commctrls background, WM_CTLCOLORSTATIC 
	 */
	HBRUSH _backgroundBrush;

	short _mouseX;
	short _mouseY;

public:

	NuoWindow();
	NuoWindow(const std::string& title);
	virtual ~NuoWindow();

	static void RegisterClass();

	virtual void OnDestroy();
	virtual bool OnCommand(int id, int notification);
	virtual void OnPaint();
	virtual void OnSize(unsigned int x, unsigned int y);
	virtual void OnMouseMove(short x, short y);
	virtual void OnMouseDown(short x, short y);
	virtual void OnMouseDrag(short x, short y, short deltaX, short deltaY, const NuoMouseModifer& modifier);
	virtual void OnMouseUp(short x, short y);
	virtual bool OnScrollWheel(short keyState, short delta);
	virtual void OnDPIChange(const NuoRect<long>& newRect, float newDPI, float oldDPI);

	HWND Handle() const;

	void Show();
	void Hide();
	void Update();

	void EnableMouseDrag();
	
	void SetMenu(const PNuoMenuBar& menu);
	void SetIcon(const PNuoIcon& icon);
	virtual void SetText(const std::string& text);
	virtual NuoRect<long> PositionDevice();
	virtual void SetPositionDevice(const NuoRect<long>& pos, bool activate);
	NuoRect<long> ClientRectDevice();
	NuoRect<float> ClientRect();

	float DPI() const;
	long TextWidth(const std::string& text, const PNuoFont& font);
	long TextHeight(const std::string& text, const PNuoFont& font);

	std::shared_ptr<NuoFont> Font();
	void SetFont(const std::shared_ptr<NuoFont>& font);

	void Detach();
	void Destroy();
	void SetOnDestroy(SimpleFunc func);

	void Add(const PNuoWindow& child);

private:

	float SavedDPI() const;
	SIZE TextSize(const std::string& text, const PNuoFont& font);

	static LRESULT CALLBACK NuoWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnMouseMessage(short x, short y, WPARAM wParam);

};



