
#include "IconWindow.h"
#include <wingdi.h>

#include "NuoOpenFileDialog.h"
#include "NuoImage.h"

#include "resource.h"


IconWindow::IconWindow(const PNuoWindow& appWindow)
	: NuoWindow("Icon Tools"),
	  _appWindow(appWindow)
{
}


void IconWindow::Init()
{
	NuoRect<float> rect(0, 0, 120, 24);

	_loadButton = std::make_shared<NuoButton>(shared_from_this(), "Load Image ...");
	_loadButton->Init(IDB_LOADICON);
	_loadButton->SetAutoPosition(kNuoControl_RT);
	_loadButton->SetPosition(rect, false);

	_loadButton->SetOnCommand([this]()
		{
			NuoFileDialog dlg;
			dlg.Open(this->shared_from_this());

			std::string path = dlg.FilePath();

			PNuoImage image = std::make_shared<NuoImage>();
			image->Load(path, this->DPI() * 20);

			this->_iconLabel->SetImage(image);
		});

	rect.SetY(rect.H() + 20 + 8);
	_saveButton = std::make_shared<NuoButton>(shared_from_this(), "Save Icon ...");
	_saveButton->Init(IDB_SAVEICON);
	_saveButton->SetAutoPosition(kNuoControl_R);
	_saveButton->SetPosition(rect, false);

	_saveButton->SetOnCommand([this]()
		{
			NuoFileDialog dlg;
			dlg.Save(this->shared_from_this());

			std::string path = dlg.FilePath();

			PNuoImage image = this->_iconLabel->Image();
			PNuoIcon icon = image->Icon();
			icon->Save(path);
		});

	auto font = std::make_shared<NuoFont>(16, "MS Shell Dlg");
	font->SetItalic(false);
	font->SetLight(true);
	font->CreateFont(DPI());
	_loadButton->SetFont(font);
	_saveButton->SetFont(font);

	NuoInset<float> labelInset(30, 20, 35, 165);
	_iconLabel = std::make_shared<NuoLabel>(shared_from_this());
	_iconLabel->Init(true);
	_iconLabel->SetAutoPosition(kNuoControl_Stretch_ALL);
	_iconLabel->SetMargin(labelInset);
}


NuoRect<long> IconWindow::PreferredRect() const
{
	PNuoWindow appWindow = _appWindow.lock();
	NuoRect<long> appRect = appWindow->PositionDevice();

	float scale = DPI();
	NuoRect<long> result(0, 0, 400 * scale, 300 * scale);
	result.SetX(appRect.X() + 30 * scale);
	result.SetY(appRect.Y() + 30 * scale);

	return result;
}

