
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


typedef std::function<void()> CommandFunc;


class NuoControl : public NuoWindow
{

protected:

	NuoControlAutoPosition _autoPosition;

	std::weak_ptr<NuoWindow> _parent;
	int _id;

	CommandFunc _commandFunc;

public:

	NuoControl(const std::string& title, const PNuoWindow& parent);
	virtual ~NuoControl();

	void SetFocus();
	virtual NuoRect<long> PositionDevice() override;

	void SetAutoPosition(NuoControlAutoPosition autoPos);
	NuoControlAutoPosition AutoPosition() const;

	int ID() const;

	virtual NuoRect<long> AutoPositionDevice(float scale, NuoRect<long> parentBound);

	virtual void OnCommand();
	virtual void SetOnCommand(const CommandFunc& func);
};


typedef std::shared_ptr<NuoControl> PNuoControl;

