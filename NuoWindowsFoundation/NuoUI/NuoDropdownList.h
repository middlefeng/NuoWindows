
//  NuoDropdownList.h
//  ModelViewer
//
//  Created by Dong Feng on 12/19/2020.
//  Copyright © 2020 Dong Feng. All rights reserved.
//


#ifndef __NUO_DROPDOWN_LIST__
#define __NUO_DROPDOWN_LIST__


#include <windows.h>

#include <string>
#include <vector>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoControl.h"


class NuoDialog;
typedef std::shared_ptr<NuoDialog> PNuoDialog;


class NuoDropdownList : public NuoControl
{

	std::vector<std::string> _itemList;
	std::string _selectedItem;

public:

	NuoDropdownList(const PNuoWindow& parent, const std::vector<std::string>& list);
	virtual ~NuoDropdownList();

	void Init(int controlID);
	void SelectItem(const std::string& item);
	const std::string& SelectedItem();

	// a dropdown's position does not behave like a normal window/view's. its height
	// affect the dropped's height instead of the control its own.
	//
	virtual void SetPosition(const NuoRect<float>& pos, bool activate) override;

	virtual void OnCommand(int notification) override;

	friend NuoDialog;

private:

	void UpdateDroppedWidth();

};


typedef std::shared_ptr<NuoDropdownList> PNuoDropdownList;


#endif
