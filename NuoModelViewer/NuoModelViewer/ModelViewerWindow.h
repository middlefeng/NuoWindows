#pragma once

#include "NuoWindow.h"
#include "ModelView.h"
#include <functional>


class ModelViewConfiguration;
typedef std::shared_ptr<ModelViewConfiguration> PModelViewConfiguration;

class NuoDropdownList;
typedef std::shared_ptr<NuoDropdownList> PNuoDropdownList;

class NuoProgressBar;
typedef std::shared_ptr<NuoProgressBar> PNuoProgressBar;

class ModelOperationPanel;
typedef std::shared_ptr<ModelOperationPanel> PModelOperationPanel;


class ModelViewerWindow : public NuoWindow
{

	std::function<void()> _onPaint;
	PModelView _dxView;

	PModelViewConfiguration _configuration;
	PNuoDropdownList _deviceList;
	PNuoProgressBar _loadingProgress;

	PModelOperationPanel _modelPanel;

public:

	void Init();
	void UpdateControls();

	ModelViewerWindow(const std::string& title);
	virtual void OnPaint();

private:

	void OpenFile();

};



typedef std::shared_ptr<ModelViewerWindow> PNuoDirectWindow;
