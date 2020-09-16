#pragma once

#include "NuoWindow.h"
#include "resource.h"

class IconWindow : public NuoWindow
{
	std::weak_ptr<NuoWindow> _appWindow;

public:
	IconWindow(const PNuoWindow& appWindow);
	NuoRect<long> PreferredRect() const;
};


typedef std::shared_ptr<IconWindow> PIconWindow;
