
#include "NuoListView.h"

#include <CommCtrl.h>

#include "NuoStrings.h"
#include "NuoAppInstance.h"
#include "NuoDialog.h"


NuoListView::NuoListView(const PNuoWindow& parent)
	: NuoControl("", parent)
{
}


void NuoListView::Init(int controlID)
{
    PNuoWindow parent = _parent.lock();

    INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    int flag = WS_CHILD | WS_BORDER | LVS_REPORT | 
		       LVS_NOSORTHEADER |
		LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS | LVS_AUTOARRANGE | LVS_SINGLESEL;

    _hWnd = CreateWindow(WC_LISTVIEW,     // Predefined class; Unicode assumed 
                         L"",
                         flag,                  // Styles 
                         10, 10, 300, 40,       // Button height
                         parent->Handle(),      // Parent window
                         nullptr, //(HMENU)controlID,
                         NuoAppInstance::GetInstance()->Instance(),
                         NULL);

    SetWindowLongPtr(_hWnd, kWindowPtr, (LONG_PTR)this);
    parent->Add(shared_from_this());

	DWORD exStyle = ListView_GetExtendedListViewStyle(_hWnd);
	exStyle |= LVS_EX_FULLROWSELECT | LVS_EX_BORDERSELECT | LVS_EX_CHECKBOXES;
	ListView_SetExtendedListViewStyle(_hWnd, exStyle);

	WCHAR szText[256];     // Temporary buffer.
	LVCOLUMN lvc;
	int iCol;

	wsprintf(szText, L"Test Col");

	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_WIDTH | LVCF_TEXT; //  LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	// Add the columns.
	for (iCol = 0; iCol < 3; iCol++)
	{
		lvc.iSubItem = iCol;
		lvc.pszText = szText;
		lvc.cx = 100;               // Width of column in pixels.

		if (iCol < 2)
			lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
		else
			lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.

		// Load the names of the column headings from the string resources.


		// Insert the columns into the list view.
		if (ListView_InsertColumn(_hWnd, iCol, &lvc) == -1)
			return;
	}

	LVITEM item;
	item.mask = LVIF_TEXT | LVIF_PARAM;

	wchar_t* temp = new wchar_t[20];
	wsprintf(temp, L"%s", L"test line");
	item.pszText = temp;
	item.iItem = 0;
	item.iSubItem = 0;
	item.lParam = 0;
	ListView_InsertItem(_hWnd, &item);

    Update();
    Show();
}


NuoListView::~NuoListView()
{
    if (_hWnd)
    {
        DestroyWindow(_hWnd);
        _hWnd = 0;
    }
}
