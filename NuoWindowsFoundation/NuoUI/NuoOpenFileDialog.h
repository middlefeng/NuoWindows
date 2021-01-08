#pragma once


#include <commdlg.h>
#include <windows.h>

#include "NuoWindow.h"


class NuoFileDialog
{

	OPENFILENAME _ofn;
	wchar_t _szFile[256];

public:

	NuoFileDialog();

	void Open(const PNuoWindow& owner);
	void Save(const PNuoWindow& owner);
	std::string FilePath() const;

};


