
//
//  NuoProgressBar.h
//  ModelViewer
//
//  Created by Dong Feng on 1/11/2021.
//  Copyright © 2021 Dong Feng. All rights reserved.
//

#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoControl.h"


class NuoDialog;
typedef std::shared_ptr<NuoDialog> PNuoDialog;


class NuoProgressBar : public NuoControl
{

protected:

	NuoProgressBar(const PNuoDialog& parent, int controlID);

public:

	NuoProgressBar(const PNuoWindow& parent, const std::string& title);
	virtual ~NuoProgressBar();

	void Init(int controlID);
	void SetRange(float min, float max);
	void SetBarPosition(float pos);
	float BarPosition();

	friend NuoDialog;

};


typedef std::shared_ptr<NuoProgressBar> PNuoProgressBar;

