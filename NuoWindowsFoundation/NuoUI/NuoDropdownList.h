
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

public:

	NuoDropdownList(const PNuoWindow& parent, const std::vector<std::string>& list);
	virtual ~NuoDropdownList();

	void Init(int controlID);


	friend NuoDialog;

};


typedef std::shared_ptr<NuoDropdownList> PNuoDropdownList;


#endif
