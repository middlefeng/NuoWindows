//
//  NuoDispatch.h
//  ModelViewer
//
//  Created by Dong Feng on 1/14/2021.
//  Copyright © 2021 middleware. All rights reserved.
//

#ifndef NuoDispatch_hpp
#define NuoDispatch_hpp


#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>


class NuoDispatch
{

	static NuoDispatch* _dispatch;
	
	std::thread _backthread;
	std::queue <std::function<void(void)>> _works;

	std::mutex _mutex;
	std::condition_variable _condition;

	std::atomic<bool> _running;

	NuoDispatch();
	~NuoDispatch();

public:

	static NuoDispatch* GetInstance();

	void DispatchAsync(std::function<void(void)> work);

private:

	void Execute();

	static void BackThreadFunc(NuoDispatch* dispatch);

};



#endif /* NuoDispatch_hpp */
