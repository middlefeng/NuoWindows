
#pragma once

#include "NuoDialog.h"
#include "NuoButton.h"
#include "NuoLabel.h"


class AppAboutDialog : public NuoDialog
{

	PNuoLabel _label;
	PNuoLabel _labelCopyright;

public:

	AppAboutDialog(const std::string& title);
	virtual ~AppAboutDialog();

	virtual void InitDialog() override;
	virtual void UpdateLayout() override;

};


typedef std::shared_ptr<AppAboutDialog> PAppAboutDialog;

