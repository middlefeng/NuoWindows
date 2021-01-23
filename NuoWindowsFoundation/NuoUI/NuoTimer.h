
#pragma once

#include <Windows.h>

#include <string>
#include <set>
#include <functional>
#include <memory>


class NuoWindow;
typedef std::shared_ptr<NuoWindow> PNuoWindow;

class NuoTimer;
typedef std::shared_ptr<NuoTimer> PNuoTimer;
typedef std::weak_ptr<NuoTimer> WPNuoTimer;


class NuoTimer : std::enable_shared_from_this<NuoTimer>
{
public:

	typedef std::function<bool (NuoTimer* timer)> Func;

	UINT_PTR _id;

	unsigned int _interval;
	Func _function;

	NuoTimer(unsigned int _interval, Func timerFunc);

public:

	friend class NuoWindow;

	static PNuoTimer MakeTimer(unsigned int interval, Func timeFunc);
	virtual bool OnTimer();

	virtual ~NuoTimer();
};


