
#pragma once

#include <Windows.h>

#include "NuoWindow.h"

#include <string>


class NuoDialog
{
	std::string _title;

	short _x;
	short _y;
	short _cx;
	short _cy;

	HWND _hDlg;

public:

	NuoDialog(const std::string& title);

	void SetPosition(short x, short y, short cx, short cy);
	void DoModal(const PNuoWindow& parent);
};