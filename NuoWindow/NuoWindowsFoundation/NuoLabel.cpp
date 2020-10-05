
#include "NuoLabel.h"

#include "NuoStrings.h"
#include "NuoAppInstance.h"
#include "NuoDialog.h"


NuoLabel::NuoLabel(const PNuoWindow& parent)
	: NuoControl("", parent)
{
}


void NuoLabel::Init(bool image)
{
    PNuoWindow parent = _parent.lock();

    int flag = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT;
    if (image)
        flag |= SS_BITMAP;

    _hWnd = CreateWindow(L"STATIC",     // Predefined class; Unicode assumed 
                         NULL,          // Button text 
                         flag,          // Styles 
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


void NuoLabel::SetImage(const PNuoImage& image)
{
    _image = image;

    SendMessage(_hWnd, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)((HBITMAP)(*_image)));
}


NuoLabel::~NuoLabel()
{
    if (_hWnd)
    {
        DestroyWindow(_hWnd);
        _hWnd = 0;
    }
}
