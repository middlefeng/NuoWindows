// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"

#include "Resource.h"
#include "ModelViewerWindow.h"

#include "NuoMenu.h"
#include "NuoOpenFileDialog.h"
#include "NuoDropdownList.h"
#include "NuoAppInstance.h"
#include "NuoStrings.h"

#include "ModelState/ModelViewConfiguration.h"


ModelViewerWindow::ModelViewerWindow(const std::string& title)
	: NuoWindow(title)
{
	PNuoMenuBar menu = std::make_shared<NuoMenuBar>();

	PNuoMenu fileMenu = std::make_shared<NuoMenu>("  &File ");
	PNuoMenuItem fileOpenItem = std::make_shared<NuoMenuItem>(IDM_FILE_OPEN, "&Open...");
	PNuoMenuItem exitItem = std::make_shared<NuoMenuItem>(IDM_EXIT, "E&xit");
	fileMenu->AppenMenuItem(fileOpenItem);
	fileMenu->AppenMenuItem(exitItem);
	fileMenu->Update();

	menu->AppendMenu(fileMenu);
	menu->Update();

	exitItem->SetAction([this](const PNuoMenuItem&)
		{
			Destroy();
			NuoAppInstance::Exit();
		});

	fileOpenItem->SetAction([this](const PNuoMenuItem&)
		{
			NuoFileDialog dlg;
			dlg.Open(this->shared_from_this());

			_dxView->OpenFile(dlg.FilePath());
		});

	SetMenu(menu);

	NuoAppInstance* app = NuoAppInstance::GetInstance();
	std::string path = RemoveLastPathComponent(app->ModulePath());
	_configuration = std::make_shared<ModelViewConfiguration>(path + "/configuration.lua");
}


void ModelViewerWindow::Init()
{
	PNuoDevice currentDevice = _configuration->Device();

	currentDevice->EnableDebugInfoQueue();

	_dxView = std::make_shared<ModelView>(currentDevice, shared_from_this());
	Add(_dxView);

	NuoInset<float> margin(0, 0, 0, 200);
	_dxView->SetAutoPosition(kNuoControl_Stretch_ALL);
	_dxView->SetMargin(margin);

	NuoInset<float> deivceListMargin(0, 0, 20, 20);
	NuoRect<float> deviceListPos(0, 0, 150, 100);
	_deviceList = std::make_shared<NuoDropdownList>(shared_from_this(),
													_configuration->DeviceNames());
	Add(_deviceList);

	std::weak_ptr<ModelViewConfiguration> configuration = _configuration;
	std::weak_ptr<NuoDropdownList> list = _deviceList;

	_deviceList->Init(IDC_DEVICE_LIST);
	_deviceList->SetAutoPosition(kNuoControl_RB);
	_deviceList->SetMargin(deivceListMargin);
	_deviceList->SetPosition(deviceListPos, false);
	_deviceList->SetFont(NuoFont::MenuFont(16.5));
	_deviceList->SetOnCommand([configuration, list](int notification)
		{
			PNuoDropdownList pList = list.lock();
			PModelViewConfiguration pConfiguration = configuration.lock();
			pConfiguration->SelectDevice(pList->SelectedItem());
		});

	UpdateControls();
}


void ModelViewerWindow::UpdateControls()
{
	_deviceList->SelectItem(_configuration->Device()->Name());
}


void ModelViewerWindow::OnPaint()
{
}

