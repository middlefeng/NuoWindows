#pragma once

#include "NuoWindow.h"
#include "resource.h"

class IconWindow : public NuoWindow
{
public:
	IconWindow();
};


typedef std::shared_ptr<IconWindow> PIconWindow;
