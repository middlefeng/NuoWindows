
#pragma once

#include "NuoDialog.h"
#include "NuoButton.h"


class AppAboutDialog : public NuoDialog
{

	PNuoButton _button;

public:

	AppAboutDialog(const std::string& title);
	virtual ~AppAboutDialog();

	virtual void UpdateLayout() override;

};


typedef std::shared_ptr<AppAboutDialog> PAppAboutDialog;

