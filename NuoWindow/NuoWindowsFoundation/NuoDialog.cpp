

#include "NuoDialog.h"

#include "NuoStrings.h"
#include "NuoAppInstance.h"

#include "FoundationResource.h"



static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)false;
}


NuoDialog::NuoDialog(const std::string& title)
	: _title(title)
{
}



void NuoDialog::SetPosition(short x, short y, short cx, short cy)
{
	_x = x; _y = y;
	_cx = cx;
	_cy = cy;
}


static const int kWindowPtr = GWLP_USERDATA;


void NuoDialog::DoModal(const PNuoWindow& parent)
{
	_hDlg = CreateDialogParam(NuoAppInstance::GetInstance()->Instance(),
							  MAKEINTRESOURCE(IDD_DIALOG_FOUNDATION), parent->Handle(), DialogProc, 0);

	SetWindowLongPtr(_hDlg, kWindowPtr, (LONG_PTR)this);

	std::wstring wtitle = StringToUTF16(_title);
	SetWindowText(_hDlg, wtitle.c_str());
	SetWindowPos(_hDlg, HWND_TOPMOST, _x, _y, _cx, _cy, SWP_SHOWWINDOW);
	ShowWindow(_hDlg, SW_SHOW);

	EnableWindow(parent->Handle(), false);

	BOOL ret;
	MSG msg;
	while ((ret = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (ret == -1) /* error found */
			break;

		if (!IsDialogMessage(_hDlg, &msg))
		{
			TranslateMessage(&msg); /* translate virtual-key messages */
			DispatchMessage(&msg);  /* send it to dialog procedure */
		}
	}

	EnableWindow(parent->Handle(), true);
}