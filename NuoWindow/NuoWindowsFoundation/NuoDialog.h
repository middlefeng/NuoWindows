
#pragma once

#include <Windows.h>

#include "NuoWindow.h"

#include <string>


class NuoDialog
{
	std::string _title;

	long _x;
	long _y;
	long _cx;
	long _cy;

	HWND _hDlg;

public:

	NuoDialog(const std::string& title);

	void SetPosition(const NuoRect<long>& pos);
	void SetPosition(long x, long y, long cx, long cy);
	void DoModal(const PNuoWindow& parent);

	virtual void UpdateLayout();

};