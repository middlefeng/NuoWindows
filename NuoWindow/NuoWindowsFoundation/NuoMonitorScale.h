#pragma once

#include <windows.h>
#include <shellscalingapi.h>

#include <NuoWindow.h>


template <class T>
double MonitorScale(const NuoPoint<T>& point)
{
	POINT pt;
	pt.x = point.X();
	pt.y = point.Y();

	DEVICE_SCALE_FACTOR scale = SCALE_100_PERCENT;

	HMONITOR monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
	GetScaleFactorForMonitor(monitor, &scale);

	double scaleF = (double)scale / 100.0;

	return scaleF;
}

