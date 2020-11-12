#pragma once

#include "NuoWindow.h"
#include "ModelView.h"
#include <functional>

class ModelViewerWindow : public NuoWindow
{

	std::function<void()> _onPaint;
	PDirectView _dxView;

public:

	void Init();

	ModelViewerWindow(const std::string& title);
	virtual void OnPaint();

};



typedef std::shared_ptr<ModelViewerWindow> PNuoDirectWindow;
