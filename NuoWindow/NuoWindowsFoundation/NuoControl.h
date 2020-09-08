
#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoWindow.h"


class NuoControl : public NuoWindow
{

protected:

	std::weak_ptr<NuoWindow> _parent;

public:

	NuoControl(const std::string& title, const PNuoWindow& parent);
	virtual ~NuoControl();

	void SetFocus();
	virtual NuoRect<long> PositionDevice() override;

};


typedef std::shared_ptr<NuoControl> PNuoControl;

