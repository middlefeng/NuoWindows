
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

#include <CommCtrl.h>


NuoDropdownList::NuoDropdownList(const PNuoWindow& parent, const std::vector<std::string>& list)
	: NuoControl("", parent), _itemList(list)
{
}


NuoDropdownList::~NuoDropdownList()
{
}



void NuoDropdownList::Init(int controlID)
{
    PNuoWindow parent = _parent.lock();

    _hWnd = CreateWindow(WC_COMBOBOX,           // Predefined class; Unicode assumed 
                         L"",                   // Button text 
                         WS_TABSTOP | WS_VISIBLE | WS_CHILD |
                         CBS_DROPDOWNLIST | CBS_HASSTRINGS,  // Styles 
                         10, 10, 300, 40,       // Button height
                         parent->Handle(),      // Parent window
                         (HMENU)controlID,             
                         NuoAppInstance::GetInstance()->Instance(),
                         NULL);

    for (const std::string& item : _itemList)
    {
        std::wstring witem = StringToUTF16(item);
        SendMessage(_hWnd, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)witem.c_str());
    }

    _id = controlID;

    SetWindowLongPtr(_hWnd, kWindowPtr, (LONG_PTR)this);
    parent->Add(shared_from_this());

    Update();
    Show();
}


void NuoDropdownList::SelectItem(const std::string& item)
{
    auto pos = std::find(_itemList.begin(), _itemList.end(), item);
    if (pos != _itemList.end())
    {
        unsigned int i = pos - _itemList.begin();
        SendMessage(_hWnd, CB_SETCURSEL, i, 0);

        _selectedItem = item;
    }
}


const std::string& NuoDropdownList::SelectedItem()
{
    return _selectedItem;
}


void NuoDropdownList::SetPosition(const NuoRect<float>& pos, bool activate)
{
    NuoRect<float> aPos = pos;
    aPos.SetH(1000);

    NuoControl::SetPosition(aPos, activate);
    UpdateDroppedWidth();
}


void NuoDropdownList::OnCommand(int notification)
{
    if (notification == CBN_SELENDOK)
    {
        LRESULT index = SendMessage(_hWnd, CB_GETCURSEL, 0, 0);
        LRESULT length = SendMessage(_hWnd, CB_GETLBTEXTLEN, index, 0);

        std::vector<wchar_t> buf(length + 1);

        SendMessage(_hWnd, CB_GETLBTEXT, index, (LPARAM)buf.data());
        
        std::string value = StringToUTF8(buf.data());
        if (_selectedItem != value)
        {
            _selectedItem = value;
            NuoControl::OnCommand(notification);
        }
    }
}


void NuoDropdownList::UpdateDroppedWidth()
{
    auto font = Font();
    long maxWidth = 0;

    for (const std::string& item : _itemList)
    {
        long width = TextWidth(item, font);
        maxWidth = width > maxWidth ? width : maxWidth;
    }

    SendMessage(_hWnd, CB_SETDROPPEDWIDTH, maxWidth + 10, 0);
}

