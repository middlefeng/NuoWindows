
#include "IconWindow.h"
#include <wingdi.h>

#include "NuoOpenFileDialog.h"
#include "resource.h"


IconWindow::IconWindow(const PNuoWindow& appWindow)
	: NuoWindow("Icon Tools"),
	  _appWindow(appWindow)
{
}


void IconWindow::Init()
{
	NuoRect<long> rect(0, 0, 120, 24);

	_loadButton = std::make_shared<NuoButton>(shared_from_this(), "Load Icon ...");
	_loadButton->Init(IDB_LOADICON);
	_loadButton->SetAutoPosition(kNuoControl_RT);
	_loadButton->SetPosition(rect, false);

	_loadButton->SetOnCommand([this]()
		{
			NuoOpenFileDialog dlg;
			dlg.Open(this->shared_from_this());

			std::string result = dlg.FilePath();
		});

	auto font = std::make_shared<NuoFont>(16, "MS Shell Dlg");
	font->SetItalic(false);
	font->SetLight(true);
	font->CreateFont(DPI());
	_loadButton->SetFont(font);

	NuoRect<long> labelRect(0, 0, 100, 100);
	_iconLabel = std::make_shared<NuoLabel>(shared_from_this());
	_iconLabel->Init();
	_iconLabel->SetAutoPosition(kNuoControl_LT_Stretch);
	_iconLabel->SetPosition(labelRect, false);
	_iconLabel->SetText("test");
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

