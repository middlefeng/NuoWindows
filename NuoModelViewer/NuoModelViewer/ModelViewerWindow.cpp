// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"

#include "Resource.h"
#include "ModelViewerWindow.h"

#include "NuoMenu.h"
#include "NuoProgressBar.h"
#include "NuoOpenFileDialog.h"
#include "NuoDropdownList.h"
#include "NuoAppInstance.h"
#include "NuoStrings.h"

#include "ModelState/ModelState.h"
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
			this->OpenFile();
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

	NuoInset<float> loadingProgressMaring(20, 0, 20, 0);
	NuoRect<float> loadingProgressPos(0, 0, 150, 30);
	_loadingProgress = std::make_shared<NuoProgressBar>(shared_from_this(), "");
	Add(_loadingProgress);

	_loadingProgress->Init(0);
	_loadingProgress->SetAutoPosition(kNuoControl_LB);
	_loadingProgress->SetMargin(loadingProgressMaring);
	_loadingProgress->SetPosition(loadingProgressPos, true);
	_loadingProgress->SetFont(NuoFont::MenuFont(16.5));
	_loadingProgress->SetRange(0.0, 1.0);
}


void ModelViewerWindow::UpdateControls()
{
	_deviceList->SelectItem(_configuration->Device()->Name());
}


void ModelViewerWindow::OnPaint()
{
}


void ModelViewerWindow::OpenFile()
{
	NuoFileDialog dlg;
	dlg.Open(shared_from_this());

	std::weak_ptr<ModelView> dxView = _dxView;
	PNuoProgressBar progressBar = _loadingProgress;

	_dxView->Hide();
	_loadingProgress->Show();

	_dxView->OpenFile(dlg.FilePath(), [progressBar](float p)
		{
			progressBar->SetBarPosition(p);
		});

	_loadingProgress->Hide();
	_dxView->Show();

	PModelState modelState = _dxView->State();

	SetText("  Model Viewer - " + modelState->DocumentName());
}

