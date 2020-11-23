
#include "NuoListView.h"

#include <CommCtrl.h>

#include "NuoStrings.h"
#include "NuoAppInstance.h"
#include "NuoDialog.h"

#include <vector>


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
		       LVS_NOSORTHEADER  |
			   LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS | 
			   LVS_AUTOARRANGE | LVS_SINGLESEL;

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
	exStyle |= LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES;
	ListView_SetExtendedListViewStyle(_hWnd, exStyle);

    Update();
    Show();
}


void NuoListView::AddColumn(unsigned int index, const std::string& title,
							unsigned int width, NuoAlign align)
{
	LVCOLUMN lvc;

	std::wstring wtitle = StringToUTF16(title);
	std::vector<wchar_t> wstrtitle(wtitle.length() + 1, '\0');
	lstrcpyn(wstrtitle.data(), wtitle.c_str(), wtitle.length() + 1);

	width = width * DPI();

	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_WIDTH | LVCF_TEXT; //  LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	// Add the columns.
	lvc.iSubItem = index;
	lvc.pszText = wstrtitle.data();
	lvc.cx = width;               // Width of column in pixels.

	if (align == kNuoAlign_Left)
		lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
	else
		lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.

	// Insert the columns into the list view.
	ListView_InsertColumn(_hWnd, index, &lvc);
}


void NuoListView::AddItem(unsigned int index, unsigned int col, const std::string& str)
{
	LVITEM item;
	item.mask = LVIF_TEXT;

	std::wstring wstring = StringToUTF16(str);
	std::vector<wchar_t> wstr(wstring.length() + 1, '\0');
	lstrcpyn(wstr.data(), wstring.c_str(), str.length() + 1);

	item.pszText = wstr.data();
	item.iItem = index;
	item.iSubItem = col;
	item.lParam = 0;

	if (item.iSubItem)
		ListView_SetItem(_hWnd, &item);
	else
		ListView_InsertItem(_hWnd, &item);
}


NuoListView::~NuoListView()
{
    if (_hWnd)
    {
        DestroyWindow(_hWnd);
        _hWnd = 0;
    }
}
