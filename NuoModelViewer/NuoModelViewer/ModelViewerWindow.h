#pragma once

#include "NuoWindow.h"
#include "ModelView.h"
#include <functional>


class ModelViewConfiguration;
typedef std::shared_ptr<ModelViewConfiguration> PModelViewConfiguration;

class NuoDropdownList;
typedef std::shared_ptr<NuoDropdownList> PNuoDropdownList;


class ModelViewerWindow : public NuoWindow
{

	std::function<void()> _onPaint;
	PDirectView _dxView;

	PModelViewConfiguration _configuration;
	PNuoDropdownList _deviceList;

public:

	void Init();

	ModelViewerWindow(const std::string& title);
	virtual void OnPaint();

};



typedef std::shared_ptr<ModelViewerWindow> PNuoDirectWindow;
