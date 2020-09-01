
#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoWindow.h"


class NuoControl : public NuoWindow
{

public:

	NuoControl(const std::string& title);
	virtual ~NuoControl();

};


typedef std::shared_ptr<NuoControl> PNuoControl;

