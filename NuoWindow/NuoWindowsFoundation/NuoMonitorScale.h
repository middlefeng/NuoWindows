#pragma once

#include <windows.h>
#include <shellscalingapi.h>

#include <NuoWindow.h>


template <class T>
NuoPoint<T> MonitorDPI(const NuoPoint<T>& point)
{
	POINT pt;
	pt.x = (LONG)point.X();
	pt.y = (LONG)point.Y();

	UINT dpiX, dpiY;
	HMONITOR monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
	GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);

	NuoPoint<T> scaleF(1, 1);
	scaleF.SetX((double)dpiX / 100.0);
	scaleF.SetY((double)dpiY / 100.0);

	return scaleF;
}

