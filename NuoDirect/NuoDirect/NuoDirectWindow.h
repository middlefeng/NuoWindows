#pragma once

#include "NuoWindow.h"
#include "DirectView.h"
#include <functional>

class NuoDirectWindow : public NuoWindow
{

	std::function<void()> _onPaint;
	PDirectView _dxView;

public:

	void Init();

	NuoDirectWindow(const std::string& title);
	virtual void OnPaint();

};



typedef std::shared_ptr<NuoDirectWindow> PNuoDirectWindow;
