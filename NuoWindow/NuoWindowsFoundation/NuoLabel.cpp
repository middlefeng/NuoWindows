
#include "NuoLabel.h"

#include "NuoStrings.h"
#include "NuoAppInstance.h"
#include "NuoDialog.h"


NuoLabel::NuoLabel(const PNuoWindow& parent)
	: NuoControl("", parent)
{
}


void NuoLabel::Init()
{
    PNuoWindow parent = _parent.lock();

    _hWnd = CreateWindow(L"STATIC",     // Predefined class; Unicode assumed 
                         NULL,          // Button text 
                         WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,  // Styles 
                         10, 10, 300, 40,       // Button height
                         parent->Handle(),      // Parent window
                         NULL,                  // No menu.
                         NuoAppInstance::GetInstance()->Instance(),
                         NULL);

    SetWindowLongPtr(_hWnd, kWindowPtr, (LONG_PTR)this);
    parent->Add(shared_from_this());

    Update();
    Show();
}


void NuoLabel::SetText(const std::string& text)
{
    std::wstring wtext = StringToUTF16(text);

    SetWindowText(_hWnd, wtext.c_str());
}


NuoLabel::~NuoLabel()
{
    if (_hWnd)
    {
        DestroyWindow(_hWnd);
        _hWnd = 0;
    }
}
