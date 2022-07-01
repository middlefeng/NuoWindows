
//
//  NuoScrollView.h
//  ModelViewer
//
//  Created by Dong Feng on 4/26/22.
//  Copyright © 2022 Dong Feng. All rights reserved.
//

#ifndef __NUO_SCROLLVIEW_H__
#define __NUO_SCROLLVIEW_H__

#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoControl.h"


class NuoDialog;
typedef std::shared_ptr<NuoDialog> PNuoDialog;


class NuoScrollView : public NuoControl
{

protected:

	NuoScrollView(const PNuoDialog& parent, int controlID);

public:

	NuoScrollView(const PNuoWindow& parent, const std::string& title);
	virtual ~NuoScrollView();

	void Init(int controlID);

	void SetContentHeight(float height);
	int ScrollPosDevice() const;
	void SetScrollPosDevice(int pos);

	friend NuoDialog;

private:

	void OnScroll(UINT message, WPARAM wParam, LPARAM lParam);
	virtual bool OnScrollWheel(short keyState, short delta) override;

	friend NuoWindow;

};


typedef std::shared_ptr<NuoScrollView> PNuoScrollView;


typedef std::shared_ptr<NuoControl> PNuoControl;


#endif
