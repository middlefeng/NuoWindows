
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
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    int flag = WS_CHILD | LVS_REPORT | LVS_EDITLABELS;

    _hWnd = CreateWindow(WC_LISTVIEW,     // Predefined class; Unicode assumed 
                         L"",
                         flag,                  // Styles 
                         10, 10, 300, 40,       // Button height
                         parent->Handle(),      // Parent window
                         (HMENU)controlID,
                         NuoAppInstance::GetInstance()->Instance(),
                         NULL);

    SetWindowLongPtr(_hWnd, kWindowPtr, (LONG_PTR)this);
    parent->Add(shared_from_this());

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
