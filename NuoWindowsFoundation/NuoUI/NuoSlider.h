
//
//  NuoSlider.h
//  ModelViewer
//
//  Created by Dong Feng on 4/24/22.
//  Copyright © 2022 Dong Feng. All rights reserved.
//

#ifndef __NUO_SLIDER_H__
#define __NUO_SLIDER_H__

#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoControl.h"


class NuoDialog;
typedef std::shared_ptr<NuoDialog> PNuoDialog;


class NuoSlider : public NuoControl
{

protected:

	NuoSlider(const PNuoDialog& parent, int controlID);

public:

	NuoSlider(const PNuoWindow& parent, const std::string& title);
	virtual ~NuoSlider();

	void Init(int controlID);


	friend NuoDialog;

};


typedef std::shared_ptr<NuoSlider> PNuoSlider;


#endif
