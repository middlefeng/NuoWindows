
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
	NuoRect<long> pos(10, 10, 300, 100);

	_button = std::make_shared<NuoButton>(shared_from_this(), "Button");
	_button->Init();
	_button->SetPositionDevice(pos);
}

void AppAboutDialog::UpdateLayout()
{
}