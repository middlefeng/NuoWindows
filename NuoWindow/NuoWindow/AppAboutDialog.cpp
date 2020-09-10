
#include "AppAboutDialog.h"


AppAboutDialog::AppAboutDialog(const std::string& title)
	: NuoDialog(title)
{
}


AppAboutDialog::~AppAboutDialog()
{
}


void AppAboutDialog::InitDialog()
{
	NuoDialog::InitDialog();

	float dpi = DPI();
	NuoRect<long> pos(20 * dpi, 20 * dpi, 98 * dpi, 15 * dpi);

	_label = std::make_shared<NuoLabel>(shared_from_this());
	_label->Init();
	_label->SetText("Nuo Window");
	_label->SetPositionDevice(pos, false);

	pos.SetY(pos.Y() + pos.H() + 6 * dpi);
	_labelCopyright = std::make_shared<NuoLabel>(shared_from_this());
	_labelCopyright->Init();
	_labelCopyright->SetText("Copyright (c) 2020");
	_labelCopyright->SetPositionDevice(pos, false);

	SetShowCancel(false);
}

void AppAboutDialog::UpdateLayout()
{
	NuoDialog::UpdateLayout();
}