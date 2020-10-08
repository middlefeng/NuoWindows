
#include "ListViewWindow.h"
#include <wingdi.h>

#include "NuoOpenFileDialog.h"
#include "NuoImage.h"

#include "resource.h"


ListViewWindow::ListViewWindow(const PNuoWindow& appWindow)
	: NuoWindow("Icon Tools"),
	  _appWindow(appWindow)
{
}


void ListViewWindow::Init()
{
	NuoRect<float> rect(0, 0, 120, 24);

	_loadButton = std::make_shared<NuoButton>(shared_from_this(), "Load ...");
	_loadButton->Init(IDB_LOADICON);
	_loadButton->SetAutoPosition(kNuoControl_RT);
	_loadButton->SetPosition(rect, false);

	auto font = std::make_shared<NuoFont>(16, "MS Shell Dlg");
	font->SetItalic(false);
	font->SetLight(true);
	font->CreateFont(DPI());
	_loadButton->SetFont(font);

	_loadButton->SetOnCommand([this]()
		{
			
		});

}


NuoRect<long> ListViewWindow::PreferredRect() const
{
	PNuoWindow appWindow = _appWindow.lock();
	NuoRect<long> appRect = appWindow->PositionDevice();

	float scale = DPI();
	NuoRect<long> result(0, 0, 400 * scale, 300 * scale);
	result.SetX(appRect.X() + 30 * scale);
	result.SetY(appRect.Y() + 30 * scale);

	return result;
}

