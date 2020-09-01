
#pragma once

#include <Windows.h>

#include "NuoWindow.h"

#include <string>


class NuoDialogProc;

class NuoDialog : public NuoWindow
{
	long _x;
	long _y;
	long _cx;
	long _cy;

public:

	NuoDialog(const std::string& title);

	void SetPosition(const NuoRect<long>& pos);
	void SetPosition(long x, long y, long cx, long cy);
	void DoModal(const PNuoWindow& parent);

	virtual void InitDialog();
	virtual void UpdateLayout();

	friend NuoDialogProc;

};