
#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoWindow.h"


enum NuoControlAutoPosition
{
	kNuoControl_NoneAuto,
	kNuoControl_RT,
	kNuoControl_RB,
};


class NuoControl : public NuoWindow
{

protected:

	NuoControlAutoPosition _autoPosition;

	std::weak_ptr<NuoWindow> _parent;

public:

	NuoControl(const std::string& title, const PNuoWindow& parent);
	virtual ~NuoControl();

	void SetFocus();
	virtual NuoRect<long> PositionDevice() override;

	void SetAutoPosition(NuoControlAutoPosition autoPos);
	NuoControlAutoPosition AutoPosition() const;

	virtual NuoRect<long> AutoPositionDevice(float scale, NuoRect<long> parentBound);

};


typedef std::shared_ptr<NuoControl> PNuoControl;

