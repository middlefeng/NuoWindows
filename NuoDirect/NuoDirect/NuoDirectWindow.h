#pragma once

#include "NuoWindow.h"
#include <functional>

class NuoDirectWindow : public NuoWindow
{

	std::function<void()> _onPaint;

public:

	NuoDirectWindow(const std::string& title);
	virtual void OnPaint();

	void SetOnPaint(std::function<void()> onPaint);

};



typedef std::shared_ptr<NuoDirectWindow> PNuoDirectWindow;
