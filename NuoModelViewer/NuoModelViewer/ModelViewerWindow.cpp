// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "ModelViewerWindow.h"

#include "NuoAppInstance.h"


ModelViewerWindow::ModelViewerWindow(const std::string& title)
	: NuoWindow(title)
{
}


void ModelViewerWindow::Init()
{
	PNuoDevice currentDevice;
	std::set<PNuoDevice> devices = NuoDevice::Devices();
	for (auto device : devices)
	{
		std::string name = device->Name();
		if (name.find("NVIDIA") != std::string::npos)
			currentDevice = device;
	}

	currentDevice->EnableDebugInfoQueue();

	_dxView = std::make_shared<ModelView>(currentDevice, shared_from_this());
	Add(_dxView);

	NuoInset<float> margin(0, 0, 0, 200);
	_dxView->SetAutoPosition(kNuoControl_Stretch_ALL);
	_dxView->SetMargin(margin);
}


void ModelViewerWindow::OnPaint()
{
}

