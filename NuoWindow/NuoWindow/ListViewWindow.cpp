
#include "ListViewWindow.h"
//#include <wingdi.h>
#include <CommCtrl.h>

#include "NuoOpenFileDialog.h"

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
	
	_loadButton->SetFont(font);

	_loadButton->SetOnCommand([this]()
		{
			
		});

	font = std::make_shared<NuoFont>(14, "MS Shell Dlg");

	NuoInset<float> listInset(30, 20, 35, 165);
	_listView = std::make_shared<NuoListView>(shared_from_this());
	_listView->Init(IDM_LISTVIEW);
	_listView->SetAutoPosition(kNuoControl_Stretch_ALL);
	_listView->SetMargin(listInset);

	_listView->AddColumn(0, "Name", 150, kNuoAlign_Left);
	_listView->AddColumn(1, "Description", 350, kNuoAlign_Left);
	_listView->SetFont(font);

	_listView->AddItem(0, 0, "Windows"); _listView->AddItem(0, 1, "Microsoft desktop operating system.");
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

