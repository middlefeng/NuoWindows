
//  NuoDropdownList.cpp
//  ModelViewer
//
//  Created by Dong Feng on 12/19/2020.
//  Copyright © 2020 Dong Feng. All rights reserved.
//

#include "NuoDropdownList.h"

#include "NuoStrings.h"
#include "NuoAppInstance.h"
#include "NuoDialog.h"


NuoDropdownList::NuoDropdownList(const PNuoWindow& parent, const std::vector<std::string>& list)
	: NuoControl("", parent)
{
}


NuoDropdownList::~NuoDropdownList()
{
}



void NuoDropdownList::Init(int controlID)
{
    std::wstring wtitle = StringToUTF16(_title);
    PNuoWindow parent = _parent.lock();

    _hWnd = CreateWindow(L"BUTTON",     // Predefined class; Unicode assumed 
                         wtitle.c_str(),        // Button text 
                         WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,  // Styles 
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
