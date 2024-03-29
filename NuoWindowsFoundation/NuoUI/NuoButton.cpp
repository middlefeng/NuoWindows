
#include "NuoButton.h"

#include "NuoStrings.h"
#include "NuoAppInstance.h"
#include "NuoDialog.h"


NuoButton::NuoButton(const PNuoWindow& parent, const std::string& title)
	: NuoControl(title, parent)
{
}


NuoButton::NuoButton(const PNuoDialog& parent, int controlID)
    : NuoControl("", std::dynamic_pointer_cast<NuoWindow>(parent))
{
    _hWnd = GetDlgItem(parent->Handle(), controlID);
}


void NuoButton::Init(int controlID)
{
    std::wstring wtitle = StringToUTF16(_title);
    PNuoWindow parent = _parent.lock();

    _hWnd = CreateWindow(L"BUTTON",     // Predefined class; Unicode assumed 
                         wtitle.c_str(),        // Button text 
                         WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,  // Styles 
                         10, 10, 300, 40,       // Button height
                         parent->Handle(),      // Parent window
                         (HMENU)((long long)controlID),             
                         NuoAppInstance::GetInstance()->Instance(),
                         NULL);

    _id = controlID;

    SetWindowLongPtr(_hWnd, kWindowPtr, (LONG_PTR)this);
    parent->Add(shared_from_this());

    Update();
    Show();
}


NuoButton::~NuoButton()
{
    if (_hWnd)
    {
        DestroyWindow(_hWnd);
        _hWnd = 0;
    }
}
