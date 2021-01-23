//
//  NuoDispatch.cpp
//  ModelViewer
//
//  Created by Dong Feng on 1/14/2021.
//  Copyright © 2021 middleware. All rights reserved.
//

#include "NuoDispatch.h"

#include <Windows.h>
#include <processthreadsapi.h>


NuoDispatch* NuoDispatch::_dispatch = nullptr;



void NuoDispatch::BackThreadFunc(NuoDispatch* dispatch)
{
	while (dispatch->_running)
	{
		dispatch->Execute();
	}
}



NuoDispatch* NuoDispatch::GetInstance()
{
	if (!_dispatch)
	{
		_dispatch = new NuoDispatch();
	}

	return _dispatch;
}


void NuoDispatch::DispatchAsync(std::function<void(void)> work)
{
	{
		std::unique_lock<std::mutex> locker(_mutex);
		_works.push(work);
	}
	
	_condition.notify_all();
}


void NuoDispatch::Execute()
{
	SetThreadDescription(GetCurrentThread(), L"Dispatch Thread");

	std::function<void(void)> work;

	{
		std::unique_lock<std::mutex> locker(_mutex);

		_condition.wait(locker, [this]
			{
				return _works.size() > 0;
			});

		work = _works.front();
		_works.pop();
	}

	work();
}


NuoDispatch::NuoDispatch()
	: _running(true)
{
	_backthread = std::thread(BackThreadFunc, this);
}


NuoDispatch::~NuoDispatch()
{
	_running = false;
	_backthread.join();
}

