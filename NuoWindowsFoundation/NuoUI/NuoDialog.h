
#pragma once

#include <windows.h>
#include <string>

#include "NuoWindow.h"


class NuoDialogProc;
class NuoButton;

class NuoDialog : public NuoWindow
{
	long _x;
	long _y;
	long _cx;
	long _cy;

	std::shared_ptr<NuoButton> _okButton;
	std::shared_ptr<NuoButton> _cancelButton;

	bool _showCancel;

public:

	NuoDialog(const std::string& title);

	void SetPosition(const NuoRect<long>& pos);
	void SetPosition(long x, long y, long cx, long cy);
	void DoModal(const PNuoWindow& parent);

	virtual void InitDialog();
	virtual void UpdateLayout();

	void SetShowCancel(bool show);

	friend NuoDialogProc;

};


typedef std::shared_ptr<NuoDialog> PNuoDialog;