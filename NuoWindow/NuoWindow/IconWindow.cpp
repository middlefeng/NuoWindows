
#include "IconWindow.h"

IconWindow::IconWindow(const PNuoWindow& appWindow)
	: NuoWindow("Icon Tools"),
	  _appWindow(appWindow)
{
}


NuoRect<long> IconWindow::PreferredRect() const
{
	PNuoWindow appWindow = _appWindow.lock();
	NuoRect<long> appRect = appWindow->PositionDevice();

	float scale = appWindow->DPI();
	NuoRect<long> result(0, 0, 400 * scale, 300 * scale);
	result.SetX(appRect.X() + 30 * scale);
	result.SetY(appRect.Y() + 30 * scale);

	return result;
}
