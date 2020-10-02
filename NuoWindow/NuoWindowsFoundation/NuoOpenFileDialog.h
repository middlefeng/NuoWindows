#pragma once


#include <windows.h>

#include "NuoWindow.h"


class NuoOpenFileDialog
{

	OPENFILENAME _ofn;
	wchar_t _szFile[256];

public:

	NuoOpenFileDialog();

	void Open(const PNuoWindow& owner);
	std::string FilePath() const;

};


