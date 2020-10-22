// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "NuoDirectWindow.h"

#include "NuoAppInstance.h"


NuoDirectWindow::NuoDirectWindow(const std::string& title)
	: NuoWindow(title)
{
}


void NuoDirectWindow::Init()
{
	_dxView = std::make_shared<DirectView>(shared_from_this());
	Add(_dxView);

	NuoInset<float> margin(0, 0, 0, 200);
	_dxView->SetAutoPosition(kNuoControl_Stretch_ALL);
	_dxView->SetMargin(margin);
}


void NuoDirectWindow::OnPaint()
{
}


PDirectView NuoDirectWindow::DXView()
{
	return _dxView;
}
