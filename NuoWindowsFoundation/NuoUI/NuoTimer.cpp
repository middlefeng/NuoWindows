
#include "NuoWindow.h"
#include <map>



static std::map<UINT_PTR, NuoTimer*> _sTimers;



static void Timerproc(HWND hWnd, UINT msg, UINT_PTR id, DWORD Arg4)
{
	if (msg == WM_TIMER)
	{
		auto pos = _sTimers.find(id);
		if (pos != _sTimers.end())
		{
			NuoTimer* timer = pos->second;
			timer->OnTimer();
		}
	}
}



NuoTimer::NuoTimer(unsigned int interval, Func timerFunc)
	: _interval(interval),
	  _function(timerFunc)
{
	_id = SetTimer(0, 0, _interval, Timerproc);

	_sTimers.insert(std::make_pair(_id, this));
}


NuoTimer::~NuoTimer()
{
	_sTimers.erase(_id);
}


void NuoTimer::OnTimer()
{
	_function(this);
}

