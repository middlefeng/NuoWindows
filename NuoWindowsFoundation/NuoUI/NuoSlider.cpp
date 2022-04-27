
//
//  NuoSlider.cpp
//  ModelViewer
//
//  Created by Dong Feng on 4/24/22.
//  Copyright © 2022 Dong Feng. All rights reserved.
//

#include "NuoSlider.h"

#include <CommCtrl.h>
#include <windowsx.h>

#include "NuoStrings.h"
#include "NuoAppInstance.h"
#include "NuoDialog.h"


NuoSlider::NuoSlider(const PNuoWindow& parent, const std::string& title)
	: NuoControl(title, parent)
{
}


NuoSlider::NuoSlider(const PNuoDialog& parent, int controlID)
    : NuoControl("", std::dynamic_pointer_cast<NuoWindow>(parent))
{
    _hWnd = GetDlgItem(parent->Handle(), controlID);
}


void NuoSlider::Init(int controlID, int rangeMin, int rangeMax)
{
    std::wstring wtitle = StringToUTF16(_title);
    PNuoWindow parent = _parent.lock();

    _hWnd = CreateWindow(TRACKBAR_CLASS,        // Predefined class; Unicode assumed 
                         wtitle.c_str(),        // Button text 
                         WS_TABSTOP | WS_VISIBLE | WS_CHILD | TBS_NOTICKS,  // Styles 
                         10, 10, 300, 40,       // Button height
                         parent->Handle(),      // Parent window
                         (HMENU)controlID,             
                         NuoAppInstance::GetInstance()->Instance(),
                         NULL);

    SendMessage(_hWnd, (UINT)TBM_SETRANGEMIN, (WPARAM)(BOOL)FALSE, (LPARAM)rangeMin);
    SendMessage(_hWnd, (UINT)TBM_SETRANGEMAX, (WPARAM)(BOOL)FALSE, (LPARAM)rangeMax);

    ScrollBar_SetRange(_hWnd, rangeMin, rangeMax, false);

    _id = controlID;

    SetWindowLongPtr(_hWnd, kWindowPtr, (LONG_PTR)this);
    parent->Add(shared_from_this());

    Update();
    Show();
}


void NuoSlider::SetOnScroll(CommandFunc onScroll)
{
    _onScroll = onScroll;
}


void NuoSlider::OnScroll(int msg)
{
    if (_onScroll)
        _onScroll(msg);
}


void NuoSlider::SetValue(int value)
{
    SendMessage(_hWnd, (UINT)TBM_SETPOS, (WPARAM)(BOOL)true, (LPARAM)value);
}


int NuoSlider::Value() const
{
    const auto value = SendMessage(_hWnd, (UINT)TBM_GETPOS, (WPARAM)0, (LPARAM)0);
    return value;
}


NuoSlider::~NuoSlider()
{
    if (_hWnd)
    {
        DestroyWindow(_hWnd);
        _hWnd = 0;
    }
}
