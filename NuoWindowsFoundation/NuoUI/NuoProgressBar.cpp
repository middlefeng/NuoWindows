
//
//  NuoProgressBar.cpp
//  ModelViewer
//
//  Created by Dong Feng on 1/11/2021.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#include "NuoProgressBar.h"

#include <CommCtrl.h>

#include "NuoStrings.h"
#include "NuoAppInstance.h"
#include "NuoDialog.h"


NuoProgressBar::NuoProgressBar(const PNuoWindow& parent, const std::string& title)
	: NuoControl(title, parent)
{
}


NuoProgressBar::NuoProgressBar(const PNuoDialog& parent, int controlID)
    : NuoControl("", std::dynamic_pointer_cast<NuoWindow>(parent))
{
    _hWnd = GetDlgItem(parent->Handle(), controlID);
}


void NuoProgressBar::Init(int controlID)
{
    std::wstring wtitle = StringToUTF16(_title);
    PNuoWindow parent = _parent.lock();

    _hWnd = CreateWindow(PROGRESS_CLASS,     // Predefined class; Unicode assumed 
                         wtitle.c_str(),        // Button text 
                         WS_TABSTOP | WS_VISIBLE | WS_CHILD,  // Styles 
                         10, 10, 300, 40,       // Button height
                         parent->Handle(),      // Parent window
                         (HMENU)controlID,             
                         NuoAppInstance::GetInstance()->Instance(),
                         NULL);

    _id = controlID;

    SetWindowLongPtr(_hWnd, kWindowPtr, (LONG_PTR)this);
    parent->Add(shared_from_this());

    Update();
    Show();
}


void NuoProgressBar::SetRange(float min, float max)
{
    WPARAM wParam = (WPARAM)(int)(min * 1000);
    LPARAM lParam = (LPARAM)(int)(max * 1000);
    ::SendMessage(_hWnd, PBM_SETRANGE32, wParam, lParam);
}


void NuoProgressBar::SetBarPosition(float pos)
{
    WPARAM wParam = (WPARAM)(int)(pos * 1000);
    LPARAM lParam = (LPARAM)0;
    ::SendMessage(_hWnd, PBM_SETPOS, wParam, lParam);
}


float NuoProgressBar::BarPosition()
{
    UINT value = ::SendMessage(_hWnd, PBM_GETPOS, 0, 0);

    return (float)value / 1000.0f;
}


NuoProgressBar::~NuoProgressBar()
{
}
