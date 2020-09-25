
#include "NuoStrings.h"
#include "NuoOpenFileDialog.h"


NuoOpenFileDialog::NuoOpenFileDialog()
{
	memset(&_ofn, 0, sizeof(_ofn));

	auto& ofn = _ofn;

	ofn.lStructSize = sizeof(_ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = _szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(_szFile);
	ofn.lpstrFilter = L"All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}


void NuoOpenFileDialog::Open(const PNuoWindow& owner)
{
	_ofn.hwndOwner = owner->Handle();

	GetOpenFileName(&_ofn);
}



std::string NuoOpenFileDialog::FilePath() const
{
	std::wstring wfile(_ofn.lpstrFile);

	if (!wfile.size())
		return "";

	std::string result = StringToUTF8(wfile);
	return result;
}