
#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoControl.h"


class NuoButton : public NuoControl
{

	std::weak_ptr<NuoWindow> _parent;

public:

	NuoButton(const PNuoWindow& parent, const std::string& title);
	virtual ~NuoButton();

	void Init();

};


typedef std::shared_ptr<NuoButton> PNuoButton;

