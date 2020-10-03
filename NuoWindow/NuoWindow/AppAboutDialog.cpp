
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

	NuoRect<float> pos(20, 20, 98, 15);

	_label = std::make_shared<NuoLabel>(shared_from_this());
	_label->Init();
	_label->SetText("Nuo Window");
	_label->SetPosition(pos, false);

	pos.SetY((pos.Y() + pos.H() + 6));
	_labelCopyright = std::make_shared<NuoLabel>(shared_from_this());
	_labelCopyright->Init();
	_labelCopyright->SetText("Copyright (c) 2020");
	_labelCopyright->SetPosition(pos, false);

	SetShowCancel(false);
}

void AppAboutDialog::UpdateLayout()
{
	NuoDialog::UpdateLayout();
}