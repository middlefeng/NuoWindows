
#include "IconWindow.h"
#include <wingdi.h>


IconWindow::IconWindow(const PNuoWindow& appWindow)
	: NuoWindow("Icon Tools"),
	  _appWindow(appWindow)
{
}


void IconWindow::Init()
{
	float scale = DPI();
	NuoRect<long> rect(0, 0, 120 * scale, 24 * scale);

	_loadButton = std::make_shared<NuoButton>(shared_from_this(), "Load Icon ...");
	_loadButton->Init(IDB_LOADICON);
	_loadButton->SetAutoPosition(kNuoControl_RT);
	_loadButton->SetPositionDevice(rect, false);

	_loadButton->SetOnCommand([]()
		{
			
		});

	auto font = std::make_shared<NuoFont>(16, "MS Shell Dlg");
	font->SetItalic(false);
	font->SetLight(true);
	font->CreateFont(DPI());
	_loadButton->SetFont(font);
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

