
#include "NuoWindow.h"
#include <map>



static std::map<UINT_PTR, PNuoTimer> _sTimers;



static void Timerproc(HWND hWnd, UINT msg, UINT_PTR id, DWORD Arg4)
{
	if (msg == WM_TIMER)
	{
		auto pos = _sTimers.find(id);
		if (pos != _sTimers.end())
		{
			PNuoTimer timer = pos->second;
			if (!timer->OnTimer())
			{
				_sTimers.erase(pos);
			}
		}
	}
}



PNuoTimer NuoTimer::MakeTimer(unsigned int interval, Func timeFunc)
{
	return std::make_shared<NuoTimer>(interval, timeFunc);
}



NuoTimer::NuoTimer(unsigned int interval, Func timerFunc)
	: _interval(interval),
	  _function(timerFunc)
{
	_id = SetTimer(0, 0, _interval, Timerproc);

	_sTimers.insert(std::make_pair(_id, shared_from_this()));
}


NuoTimer::~NuoTimer()
{
	KillTimer(0, _id);
}


bool NuoTimer::OnTimer()
{
	return _function(this);
}

