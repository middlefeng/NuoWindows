
#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoWindow.h"


enum NuoControlAutoPosition
{
	kNuoControl_NoneAuto = 0x00,
	kNuoControl_RT,
	kNuoControl_R,
	kNuoControl_RB,

	kNuoControl_Stretch = 0x1000000,
	kNuoControl_Stretch_L = kNuoControl_Stretch | 0x0001,
	kNuoControl_Stretch_T = kNuoControl_Stretch | 0x0010,
	kNuoControl_Stretch_R = kNuoControl_Stretch | 0x0100,
	kNuoControl_Stretch_B = kNuoControl_Stretch | 0x1000,
	kNuoControl_Stretch_ALL = kNuoControl_Stretch_L | kNuoControl_Stretch_T |
							  kNuoControl_Stretch_R | kNuoControl_Stretch_B
};


typedef std::function<void(int)> CommandFunc;


class NuoControl : public NuoWindow
{

protected:

	NuoControlAutoPosition _autoPosition;

	std::weak_ptr<NuoWindow> _parent;
	int _id;

	CommandFunc _commandFunc;

	NuoInset<float> _parentMargin;

public:

	NuoControl(const std::string& title, const PNuoWindow& parent);
	virtual ~NuoControl();

	void SetFocus();
	virtual NuoRect<long> PositionDevice() override;
	virtual NuoRect<float> Position();
	virtual void SetPosition(const NuoRect<float>& pos, bool activate);

	void SetAutoPosition(NuoControlAutoPosition autoPos);
	NuoControlAutoPosition AutoPosition() const;

	void SetMargin(const NuoInset<float>& inset);

	int ID() const;

	virtual NuoRect<float> AutoPosition();

	virtual void OnCommand(int notification);
	virtual void SetOnCommand(const CommandFunc& func);
};


typedef std::shared_ptr<NuoControl> PNuoControl;

