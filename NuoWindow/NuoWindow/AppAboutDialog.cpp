
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
	NuoRect<long> pos(50, 50, 188, 153);

	_button = std::make_shared<NuoButton>(shared_from_this(), "Cancel");
	_button->Init();
	_button->SetPositionDevice(pos, false);

	NuoFont font = Font();
	_button->SetFont(font);
}

void AppAboutDialog::UpdateLayout()
{
	NuoDialog::UpdateLayout();
}