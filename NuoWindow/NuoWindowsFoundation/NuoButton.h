
#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoControl.h"


class NuoDialog;
typedef std::shared_ptr<NuoDialog> PNuoDialog;


class NuoButton : public NuoControl
{

protected:

	NuoButton(const PNuoDialog& parent, int controlID);

public:

	NuoButton(const PNuoWindow& parent, const std::string& title);
	virtual ~NuoButton();

	void Init();


	friend NuoDialog;

};


typedef std::shared_ptr<NuoButton> PNuoButton;

