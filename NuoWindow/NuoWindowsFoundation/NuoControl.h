
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
	kNuoControl_LT_Stretch,
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

	NuoInset<long> _parentMargin;

public:

	NuoControl(const std::string& title, const PNuoWindow& parent);
	virtual ~NuoControl();

	void SetFocus();
	virtual NuoRect<long> PositionDevice() override;
	virtual NuoRect<float> Position();
	virtual void SetPosition(const NuoRect<float>& pos, bool activate);

	void SetAutoPosition(NuoControlAutoPosition autoPos);
	NuoControlAutoPosition AutoPosition() const;

	void SetPosition(const NuoRect<long>& pos, bool activate);

	int ID() const;

	virtual void SetPositionDevice(const NuoRect<long>& pos, bool activate) override;
	virtual NuoRect<long> AutoPositionDevice(float scale, NuoRect<long> parentBound);

	virtual void OnCommand();
	virtual void SetOnCommand(const CommandFunc& func);
};


typedef std::shared_ptr<NuoControl> PNuoControl;

